//
// Created by leonid on 12.02.19.
//

#include <SFML/Network/IpAddress.hpp>
#include <iostream>
#include "player.h"
#include "weapon.h"
#include "handWeapon.h"
#include "grenade.h"

player::player(b2World &world, sf::Texture &Player_texture, sf::SoundBuffer& jump_buffer, int x, int y) : world(world) {

	jump_sound.setBuffer(jump_buffer);
	jump_sound.setVolume(30.f);

    movable = true;

    isPlayer = true;
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
    shape.SetAsBox((bound.getSize().x / (float) (2 * scale_factorX)) * bound.getScale().x,
                   (bound.getSize().y / (float) (2 * scale_factorX)) * bound.getScale().y);


    fixture.shape = &shape;
    fixture.density = 5.0f;

    fixture.filter.categoryBits = PLAYER;//I'm a...
    fixture.filter.maskBits = 1; //I will collide with...
    //if((categoryA & maskB)!=0 && (categoryB & maskA)!=0){
    //      collision
    //}

    fixture.friction = 0.3f;

    b2PolygonShape shape2;
    shape2.SetAsBox((bound.getSize().x / (float) (2 * scale_factorX)) * bound.getScale().x,
                    (bound.getSize().y / (float) (2 * scale_factorX)) * bound.getScale().y);
    b2FixtureDef fixture2;
    fixture2.shape = &shape2;
    fixture2.density = 0;
    fixture2.isSensor = true;


    // Add the shape to the body.


    realBody->CreateFixture(&fixture);
    realBody->CreateFixture(&fixture2);//sensor
    realBody->SetFixedRotation(true);

}

bool player::is_on_ground() {

	for(b2ContactEdge* ce = realBody->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Fixture* fixture = ce->contact->GetFixtureA();
		b2Body* body = fixture->GetBody();
		object* obj = static_cast<object *>(body->GetUserData());
		if(obj != nullptr)
		{
			if(obj->movable)
			{
				continue;
			}
		}
		b2WorldManifold mf;
		ce->contact->GetWorldManifold(&mf);
		b2Vec2 norm = mf.normal;
		if(abs(norm.x) <= 0.01 && norm.y > 0)
		{
			return true;
		}
	}
	return false;
}

int player::is_on_wall()
{
	for(b2ContactEdge* ce = realBody->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Fixture* fixture = ce->contact->GetFixtureA();
		b2Body* body = fixture->GetBody();
		object* obj = static_cast<object *>(body->GetUserData());
		if(obj != nullptr)
		{
			if(obj->movable)
			{
				continue;
			}
		}
		b2WorldManifold mf;
		ce->contact->GetWorldManifold(&mf);
		b2Vec2 norm = mf.normal;
		if(norm.x > 0.1 && abs(norm.y) <= 0.01)
		{
			return 1;
		}
		else if(norm.x < -0.1 && abs(norm.y) <= 0.01)
		{
			return -1;
		}
	}
	return 0;
}



void player::throwObject(b2Body &body) {
    body.ApplyLinearImpulseToCenter(b2Vec2(10 * direction * strength*dencity, 0), true);
}



void player::grabe(b2World &world,object*flipObject, bool is_flip) {

    if (!reachableObjects.empty()) {
        //what if object lost in hands


        if (JointToHold == nullptr) {
            if(flipObject == nullptr)
            cariedObject = *reachableObjects.begin();
            else cariedObject = flipObject;

            b2RevoluteJointDef jointDef;
            b2Body *box = cariedObject->realBody;

            if (cariedObject->direction != direction) {
                cariedObject->flip(direction);
            }

            b2Vec2 r1(direction * cariedObject->ancorPointShiftBodyAX / scale_factorX,
                      cariedObject->ancorPointShiftBodyAY / scale_factorX);

            b2Vec2 r2(cariedObject->ancorPointShiftBodyBX / scale_factorX,
                      cariedObject->ancorPointShiftBodyBY / scale_factorX);

            b2Vec2 shift(r1.x - r2.x, r1.y - r2.y);

            b2Vec2 pos(realBody->GetWorldCenter().x + shift.x,
                       realBody->GetWorldCenter().y + shift.y);

            box->SetTransform(pos, 0);
            box->SetFixedRotation(true);

            jointDef.bodyA = realBody;
            jointDef.bodyB = box;
            jointDef.collideConnected = false;

            jointDef.localAnchorA.Set(r1.x, r1.y);
            jointDef.localAnchorB.Set(r2.x, r2.y);

            b2RevoluteJoint *joint = (b2RevoluteJoint *) world.CreateJoint(&jointDef);
            grab = true;
            cariedObject->isBeingCaried = true;
            cariedObject->isBeingCariedBy = this->realBody;

            if(!is_flip)
            {
            	b2Fixture* fix = cariedObject->realBody->GetFixtureList();

				if(cariedObject->weapon_class == HandWeapon)
				{
					fix = fix->GetNext();
				}

            	b2Filter fil;
            	fil.categoryBits = 0;
            	fil.maskBits = 0;
            	fix->SetFilterData(fil);
            }




            JointToHold = joint;
        } else {
            b2Body *box = cariedObject->realBody;

            if(!is_flip)
            {
				b2Fixture* fix = cariedObject->realBody->GetFixtureList();
				if(cariedObject->weapon_class == HandWeapon)
				{
					fix = fix->GetNext();
				}

            	b2Filter fil;
                fil.categoryBits = 6;
                fil.maskBits = 1;
                fix->SetFilterData(fil);
            }



            world.DestroyJoint(JointToHold);
            if (box) {
                box->SetFixedRotation(false);
                throwObject(*box);
            }
            grab = false;
            cariedObject->isBeingCaried = false;
            cariedObject->isBeingCariedBy = nullptr;
            JointToHold = nullptr;
            cariedObject = nullptr;


        }
    }
}

void player::death(int x, int y) {
    if(cariedObject!= nullptr) {
        b2Fixture *fix = cariedObject->realBody->GetFixtureList();
        if (cariedObject->weapon_class == HandWeapon) {
            fix = fix->GetNext();
        }

        b2Filter fil;
        fil.categoryBits = 6;
        fil.maskBits = 1;
        fix->SetFilterData(fil);
    }
    if (grab) {
        b2Body *box = cariedObject->realBody;
        world.DestroyJoint(JointToHold);
        if (box) {
            box->SetFixedRotation(false);
            throwObject(*box);
        }
        grab = false;

        JointToHold = nullptr;
        cariedObject = nullptr;
    }


    realBody->SetTransform(b2Vec2(10,0),0);
}

void player::update(std::vector<sf::UdpSocket> &socket, int player_index, bool need_to_send)
{
    /*
     * Here we just perform the commands set in the key board listening time
     *
     *
     *
     *
     */
    if (need_to_send) {
        sf::IpAddress recipient = sf::IpAddress::LocalHost;
        unsigned short port;
        if (player_index == 1) {
            port = 54001;
        } else {
            port = 54000;
        }
        sf::Packet packet;
        int c = -1;
        packet << c; // тип эвента: -1
        float x1 = realBody->GetPosition().x;
        float y1 = realBody->GetPosition().y;
        float vx = realBody->GetLinearVelocity().x;
        float vy = realBody->GetLinearVelocity().y;
        packet << x1;
        packet << y1;
        packet << vx;
        packet << vy;
        socket[1].send(packet, recipient, port);
    }

    int IsOnWall = is_on_wall();
    if (is_on_ground())
    {
        remainingJumpSteps = 1;
    }
    if (moveRight)
    {
    	if(direction == -1)
    	{
    		counter = 0;
    	}
    	sprite.setTextureRect(sf::IntRect(30 + 177 * (counter / 5), 1 * 310, 177, 310));
    	counter += 1;
    	counter %= 16;
        direction = 1;
        if (JointToHold != nullptr && cariedObject->direction == -1)
        {
        	object* obj = cariedObject;
        	grabe(world, nullptr, true); //throw
        	grabe(world, obj, true); //grab again with different position
        }
        if (1/*IsOnWall >= 0*/) //not push a wall
        {
            realBody->SetLinearVelocity(b2Vec2(speed, realBody->GetLinearVelocity().y));
        }
    }
    else if (moveLeft)
    {
    	if(direction == 1)
    	{
    		counter = 0;
    	}
    	sprite.setTextureRect(sf::IntRect(40 + 177 * (counter / 5), 2 * 310, 177, 310));
    	counter += 1;
    	counter %= 16;

        direction = -1;
        if (JointToHold != nullptr && cariedObject->direction == 1 && cariedObject->weapon_class != Grenade)
        {
        	object* obj = cariedObject;
        	grabe(world, nullptr, true); //throw
        	grabe(world, obj, true); //grab again with different position

        }
        if (1/*IsOnWall <= 0*/) // not push a wall
        {

            realBody->SetLinearVelocity(b2Vec2(-speed, realBody->GetLinearVelocity().y));
        }
    }
    else if (1)
    {
    	sprite.setTextureRect(sf::IntRect(30, 0, 235, 310));
    	counter = 0;

        realBody->SetLinearVelocity(b2Vec2(0, realBody->GetLinearVelocity().y));
    }
    object::update();
}


void player::checkEvents(std::vector<sf::UdpSocket> &socket, sf::Event &event, b2World &world, int playerInd, int player_index) {

    sf::IpAddress recipient = sf::IpAddress::LocalHost;
    unsigned short port;
    if (player_index == 1) {
        port = 54001;
    } else{
        port = 54000;
    }
    sf::Packet packet;
    packet<<event.type;
    if(event.type == sf::Event::KeyPressed || event.type  == sf::Event::KeyReleased) {
        packet << event.key.code;
    }

    socket[1].send(packet, recipient, port);



    if (event.type == sf::Event::KeyPressed) {


        if (event.key.code == sf::Keyboard::Space && playerInd == 1) {
            if (grab) {
                if (cariedObject->weapon_class != NotWeapon) {

                    try {

                        if (cariedObject->weapon_class == Grenade) {
                            auto weapon = dynamic_cast<class grenade *>(cariedObject);
                            weapon->strike();
                        } else if (cariedObject->weapon_class == FireWeapon) {
                            auto weapon = dynamic_cast<class weapon *>(cariedObject);
                            weapon->strike();
                        } else if (cariedObject->weapon_class == HandWeapon) {
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

        if (event.key.code == sf::Keyboard::E && playerInd == 2) {
            if (grab) {
                if (cariedObject->weapon_class != NotWeapon) {

                    try {

                        if (cariedObject->weapon_class == Grenade) {
                            auto weapon = dynamic_cast<class grenade *>(cariedObject);
                            weapon->strike();
                        } else if (cariedObject->weapon_class == FireWeapon) {
                            auto weapon = dynamic_cast<class weapon *>(cariedObject);
                            weapon->strike();
                        } else if (cariedObject->weapon_class == HandWeapon) {
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

        if (event.key.code == sf::Keyboard::Right && playerInd == 1) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::Left && playerInd == 1) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::Up && playerInd == 1) {
            //Прыжки
            if (remainingJumpSteps > 0) {
                --remainingJumpSteps;
                if (is_on_ground()) {
                    realBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpHeight), true);
                    jump_sound.play();
                }
            }
        }

        if (event.key.code == sf::Keyboard::G && playerInd == 1) {

            grabe(world);


        }


        if (event.key.code == sf::Keyboard::D && playerInd == 2) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::A && playerInd == 2) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::W && playerInd == 2) {
            //Прыжки
            if (remainingJumpSteps > 0) {
                --remainingJumpSteps;
                if (is_on_ground()) {
                    realBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpHeight), true);
                    jump_sound.play();
                }
            }


        }

        if (event.key.code == sf::Keyboard::F && playerInd == 2) {

            grabe(world);


        }

    }


    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Up && playerInd == 1) {

        }

        if (event.key.code == sf::Keyboard::Right && playerInd == 1) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::Left && playerInd == 1) {
            moveLeft = false;
        }
        if (event.key.code == sf::Keyboard::D && playerInd == 2) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::A && playerInd == 2) {
            moveLeft = false;
        }

    }


}
