//
// Created by leonid on 12.02.19.
//

#include <SFML/Network/IpAddress.hpp>
#include <iostream>
#include "player.h"
#include "weapon.h"
#include "handWeapon.h"

player::player() {
}

player::player(b2World &world, sf::Texture &Player_texture, int x, int y) {

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
    if (cariedObject != nullptr) {
        b2Body *box = cariedObject->realBody;


        if (JointToHold == nullptr) {

            b2RevoluteJointDef jointDef;



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

            if (cariedObject->isThisHandWeapon) {

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

                world.DestroyJoint(JointToHold);
                box->SetFixedRotation(false);
                throwObject(*box);
                if (cariedObject->isThisHandWeapon) {

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

                JointToHold = nullptr;
                cariedObject = nullptr;


        }
    }
}


void player::update() {

    object::update();

    if (remainingJumpSteps > 0) {
        realBody->ApplyForce(b2Vec2(0, 1500), realBody->GetWorldCenter(), true);
        remainingJumpSteps--;
    }

    //Move;
    if (moveRight) {
        direction = 1;
        realBody->ApplyLinearImpulseToCenter(b2Vec2(3, 0), true);
    }
    if (moveLeft) {
        direction = -1;
        realBody->ApplyLinearImpulseToCenter(b2Vec2(-3, 0), true);
    }


}


void player::checkEvents(sf::Event &event, b2World &world, int playerInd) {



    if (event.type == sf::Event::KeyPressed) {


        if (event.key.code == sf::Keyboard::Space&&playerInd==1) {
            if (grab) {
                if (cariedObject->isThisFireWeapon || cariedObject->isThisHandWeapon) {

                    try {


                        if (cariedObject->isThisFireWeapon) {
                            auto weapon = dynamic_cast<class weapon *>(cariedObject);
                            weapon->strike();
                        }
                        if (cariedObject->isThisHandWeapon) {
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
                if (cariedObject->isThisFireWeapon || cariedObject->isThisHandWeapon) {

                    try {


                        if (cariedObject->isThisFireWeapon) {
                            auto weapon = dynamic_cast<class weapon *>(cariedObject);
                            weapon->strike();
                        }
                        if (cariedObject->isThisHandWeapon) {
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

            remainingJumpSteps = jumpHieght;


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


    if (event.type == sf::Event::KeyReleased) {

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
