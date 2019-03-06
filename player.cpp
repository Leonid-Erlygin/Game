//
// Created by leonid on 12.02.19.
//

#include <SFML/Network/IpAddress.hpp>
#include "player.h"


player::player() {
}

player::player(b2World &world, sf::Texture &Player_texture,int x,int y) {

    moveable = true;


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
        if (canTake) {


            if (JointToHold == nullptr) {

                b2RevoluteJointDef jointDef;

                canTake = true;
                box->SetTransform(realBody->GetPosition(), 0);
                box->SetFixedRotation(true);
                jointDef.Initialize(realBody, box,
                                    realBody->GetWorldCenter());
                b2RevoluteJoint *joint = (b2RevoluteJoint *) world.CreateJoint(&jointDef);
                JointToHold = joint;
            } else {
                cariedObject = nullptr;
            }
        } else {
            if (JointToHold != nullptr) {
                world.DestroyJoint(JointToHold);
                box->SetFixedRotation(false);
                throwObject(*box);
                JointToHold = nullptr;
                cariedObject = nullptr;
            }
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


void player::checkEvents(sf::Event &event, b2World &world, std::vector<sf::UdpSocket> &socket,int x) {
    sf::Packet packet;

    packet<<event.type;




    if (event.type == sf::Event::KeyPressed) {

        packet<<event.key.code;

        if (event.key.code == sf::Keyboard::Space){

        }

        if (event.key.code == sf::Keyboard::Right) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::Left) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::Up) {
            //Прыжки

            remainingJumpSteps = jumpHieght;


        }

        if (event.key.code == sf::Keyboard::G) {

            grabe(world);


        }
    }


    if (event.type == sf::Event::KeyReleased) {
        packet<<event.key.code;
        if (event.key.code == sf::Keyboard::Right) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::Left) {
            moveLeft = false;
        }
    }

    if(x){
        socket[0].send(packet,sf::IpAddress::LocalHost,54001);
    } else{
        socket[1].send(packet,sf::IpAddress::LocalHost,54000);
    }
    //Отправление пакета.


}
