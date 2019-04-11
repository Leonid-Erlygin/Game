//
// Created by leonid on 12.02.19.
//

#include <SFML/Network/IpAddress.hpp>
#include <iostream>
#include "player.h"
#include "weapon.h"
#include "handWeapon.h"
#include "grenade.h"

/*player::player() {
}*/

player::player(b2World &world, sf::Texture &Player_texture, int x, int y) : world(world) {

    moveable = true;

    isPlayer  = true;
    sprite.setTexture(Player_texture);
    sprite.setTextureRect(sf::IntRect(30, 0, 235, 310));
    bound.setPosition(x, y);
    sprite.scale(0.25, 0.25);

    bound.setSize(sf::Vector2f(235, 310));

    bound.scale(0.25, 0.25);
    bound.setFillColor(sf::Color::Blue);


    b2BodyDef realBodyDef;
    b2PolygonShape shape;
    b2FixtureDef fixture;

    realBodyDef.type = b2_dynamicBody;
    realBodyDef.position.Set(bound.getPosition().x / scale_factorX, bound.getPosition().y / scale_factorY);
    realBody = world.CreateBody(&realBodyDef);
    shape.SetAsBox((bound.getSize().x / (float32) (2 * scale_factorX)) * bound.getScale().x,
                   (bound.getSize().y / (float32) (2 * scale_factorX)) * bound.getScale().y);


    fixture.shape = &shape;
    fixture.density = 5.0f;

    fixture.filter.categoryBits = PLAYER;//I'm a...
    fixture.filter.maskBits = 1; //I will collide with...
    //if((categoryA & maskB)!=0 && (categoryB & maskA)!=0){
    //      collision
    //}

    fixture.friction = 0.3f;

    b2PolygonShape shape2;
    shape2.SetAsBox((bound.getSize().x / (float32) (2 * scale_factorX)) * bound.getScale().x,
                    (bound.getSize().y / (float32) (2 * scale_factorX)) * bound.getScale().y);
    b2FixtureDef fixture2;
    fixture2.shape = &shape2;
    fixture2.density = 0;
    fixture2.isSensor = true;


    // Add the shape to the body.


    realBody->CreateFixture(&fixture);
    realBody->CreateFixture(&fixture2);//sensor
    realBody->SetFixedRotation(true);
    realBody->SetUserData(this);
}

bool player::is_on_ground()
{
	float yrange = 1.75;
	float xrange = 0; //minimal distance could be considered as being on the ground
	QueryCallback callback;
	b2AABB aabb;
	b2Vec2 position = realBody->GetPosition();
	b2Vec2 lb(position.x - xrange/2, position.y - yrange/2);
	b2Vec2 ub(position.x + xrange/2, position.y + yrange/2);
	aabb.lowerBound = lb;
	aabb.upperBound = ub;
	world.QueryAABB(&callback, aabb);
	for(b2Body* b : callback.foundBodies)
	{
		if(b != realBody && b->GetPosition().y <= realBody->GetPosition().y &&
				b->GetPosition().y + 1.75 > realBody->GetPosition().y)
		//new world iterate within object and check fixture bounds
		{
			return true;
		}
	}
	return false;
}

void player::throwObject(b2Body &body) {
    body.ApplyLinearImpulseToCenter(b2Vec2(10 * direction * strength, 0), true);
}

void player::print() {
    printf("Bound height:%f Bound width:%f\nBox height:%f Box width:%f\n", bound.getSize().y * bound.getScale().y,
           bound.getSize().x * bound.getScale().x,
           (bound.getSize().y / (float32) (2 * scale_factorX)) * bound.getScale().y,
           (bound.getSize().x / (float32) (2 * scale_factorX)) * bound.getScale().x);
}


void player::grabe(b2World &world) {

    if (!reachableObjects.empty()) {
        //what if object lost in hands



        if (JointToHold == nullptr) {
            cariedObject = *reachableObjects.begin();
            b2RevoluteJointDef jointDef;
            b2Body* box = cariedObject->realBody;


            b2Vec2 r1(cariedObject->ancorPointShiftBodyAX/scale_factorX,
                      cariedObject->ancorPointShiftBodyAY/scale_factorX);

            b2Vec2 r2(cariedObject->ancorPointShiftBodyBX/scale_factorX,
                      cariedObject->ancorPointShiftBodyBY/scale_factorX );

            b2Vec2 shift(r1.x-r2.x,r1.y-r2.y);

            b2Vec2 pos(realBody->GetWorldCenter().x+shift.x,
                    realBody->GetWorldCenter().y+shift.y);

            box->SetTransform(pos, 0);
            box->SetFixedRotation(true);

            jointDef.bodyA = realBody;
            jointDef.bodyB = box;
            jointDef.collideConnected = false;

            jointDef.localAnchorA.Set(r1.x,r1.y);
            jointDef.localAnchorB.Set(r2.x,r2.y);

            b2RevoluteJoint *joint = (b2RevoluteJoint *) world.CreateJoint(&jointDef);
            grab = true;
            cariedObject->isBeingCaried = true;
            cariedObject->isBeingCariedBy = this->realBody;

            if (cariedObject->weapon_class == HandWeapon) {

                try {

                    auto weapon = dynamic_cast<class handWeapon *>(cariedObject);
                    weapon->isInHands = true;

                }
                catch (const std::bad_cast &e) {
                    std::cout << e.what() << std::endl;
                    std::cout << "Этот объект не является объектом типа Weapon\n" << std::endl;
                }
            }

            JointToHold = joint;
        } else {
                b2Body* box = cariedObject->realBody;
                world.DestroyJoint(JointToHold);
                if(box)
                {
                	box->SetFixedRotation(false);
                	throwObject(*box);
                }
                if (cariedObject->weapon_class == HandWeapon) {

                    try {

                        auto weapon = dynamic_cast<class handWeapon *>(cariedObject);
                        weapon->isInHands = false;

                    }
                    catch (const std::bad_cast &e) {
                        std::cout << e.what() << std::endl;
                        std::cout << "Этот объект не является объектом типа Weapon\n" << std::endl;
                    }
                }
                grab = false;
                cariedObject->isBeingCaried = false;
                cariedObject->isBeingCariedBy = nullptr;
                JointToHold = nullptr;
                cariedObject = nullptr;


        }
    }
}
void player::death(int x, int y){


    if (grab){
        b2Body* box = cariedObject->realBody;
        world.DestroyJoint(JointToHold);
        if(box)
        {
            box->SetFixedRotation(false);
            throwObject(*box);
        }
        grab = false;

        JointToHold = nullptr;
        cariedObject = nullptr;
    }

}

void player::update() {
    object::update();
    //Move;
    bool IsOnGround = true;//bug
    if(IsOnGround)
    {
    	remainingJumpSteps = jumpHieght; //landed on the floor!!!
    }
    if (moveRight) {
        direction = 1;
        if(IsOnGround) //not in the air
        {
        	realBody->SetLinearVelocity(b2Vec2(speed, realBody->GetLinearVelocity().y));
        }
    }
    else if (moveLeft) {
        direction = -1;
        if(IsOnGround)
        {
        	realBody->SetLinearVelocity(b2Vec2(-speed, realBody->GetLinearVelocity().y));
        }
    }
    else if(IsOnGround)
    {
    	realBody->SetLinearVelocity(b2Vec2(0, realBody->GetLinearVelocity().y));
    }
}


void player::checkEvents(sf::Event &event, b2World &world, int playerInd) {



    if (event.type == sf::Event::KeyPressed) {


        if (event.key.code == sf::Keyboard::Space&&playerInd==1) {
            if (grab) {
                if (cariedObject->weapon_class != NotWeapon) {

                    try {

                       if (cariedObject->weapon_class == Grenade) {
                    	    auto weapon = dynamic_cast<class grenade *>(cariedObject);
                    	    weapon->strike();
                    	}
                       else if (cariedObject->weapon_class == FireWeapon) {
                            auto weapon = dynamic_cast<class weapon *>(cariedObject);
                            weapon->strike();
                        }
                       else if (cariedObject->weapon_class == HandWeapon) {
                            auto weapon = dynamic_cast<class handWeapon *>(cariedObject);
                            weapon->strike();
                        }

                    }
                    catch (const std::bad_cast &e) {
                        std::cout << e.what() << std::endl;
                        std::cout << "Этот объект не является объектом типа Weapon\n" << std::endl;
                    }

                }
            }
        }

        if (event.key.code == sf::Keyboard::E&&playerInd==2) {
            if (grab) {
                if (cariedObject->weapon_class != NotWeapon) {

                    try {

                    	 if (cariedObject->weapon_class == Grenade) {
                    		 auto weapon = dynamic_cast<class grenade *>(cariedObject);
                    	     weapon->strike();
                    	 }
                    	 else if (cariedObject->weapon_class == FireWeapon) {
                    		 auto weapon = dynamic_cast<class weapon *>(cariedObject);
                    	     weapon->strike();
                    	 }
                    	 else if (cariedObject->weapon_class == HandWeapon) {
                    		 auto weapon = dynamic_cast<class handWeapon *>(cariedObject);
                    	     weapon->strike();
                    	 }

                    }
                    catch (const std::bad_cast &e) {
                        std::cout << e.what() << std::endl;
                        std::cout << "Этот объект не является объектом типа Weapon\n" << std::endl;
                    }

                }
            }
        }

        if (event.key.code == sf::Keyboard::Right&&playerInd==1) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::Left&&playerInd==1) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::Up&&playerInd==1) {
            //Прыжки
        	if(remainingJumpSteps > 0 && moveUp)
            {
        		moveUp = false;
        		--remainingJumpSteps;
        		  if(is_on_ground())
        		  {
        			  realBody->ApplyLinearImpulseToCenter(b2Vec2(0, 100), true);
        		  }
            }
        }

        if (event.key.code == sf::Keyboard::G&&playerInd==1) {

            grabe(world);

        }


        if (event.key.code == sf::Keyboard::D&&playerInd==2) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::A&&playerInd==2) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::W&&playerInd==2) {
            //Прыжки

            remainingJumpSteps = jumpHieght;


        }

        if (event.key.code == sf::Keyboard::F&&playerInd==2) {

            grabe(world);


        }
    }

//what the hall???
    if (event.type == sf::Event::KeyReleased) {
    	if (event.key.code == sf::Keyboard::Up&&playerInd==1) {
    		moveUp = true;
    	}

        if (event.key.code == sf::Keyboard::Right&&playerInd==1) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::Left&&playerInd==1) {
            moveLeft = false;
        }
        if (event.key.code == sf::Keyboard::D&&playerInd==2) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::A&&playerInd==2) {
            moveLeft = false;
        }
    }


}
