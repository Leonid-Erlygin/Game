#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include <stdio.h>
#include <iostream>
#include "Box2D/Box2D.h"
#include "object.h"
#include "MyContactListener.h"


int main() {

    MyContactListener myContactListenerInstance;
    // Define the gravity vector.
    b2Vec2 gravity(0.0f, -9.81f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    b2World world(gravity);


    world.SetContactListener(&myContactListenerInstance);//Нужно для детектирования столкновений
    /* ContactFilter filter;
     world.SetContactFilter(&filter);*/

#if 1

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::string Path_to_res = "/home/leonid/CLionProjects/Game/Resourses/";
    std::string Path_to_duck = "/home/leonid/CLionProjects/Game/Resourses/Duck Game Sprites/";
    sf::Texture textureCastle;
    if (!textureCastle.loadFromFile(Path_to_duck + "castle.png")) {
        return EXIT_FAILURE;
    }
    window.setFramerateLimit(60);

    object floor;

    floor.sprite.setTexture(textureCastle);
    floor.bound.setPosition(400, 500);
    floor.sprite.setTextureRect(sf::IntRect(0, 65, 96, 79 - 65));

    floor.moveable = false;


    floor.sprite.scale(20, 3);//Не изменяет значения границ, нужно домножать на этот коэфицент при переводе в метры



    floor.bodyInit(world);

    sf::View view1;
    view1.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    view1.setCenter(sf::Vector2f(view1.getSize().x / 2, view1.getSize().y / 2));
    window.setView(view1);


    sf::Texture textureBox;
    if (!textureBox.loadFromFile(Path_to_res + "box1.png")) {
        return EXIT_FAILURE;
    }

    sf::Texture textureBarrel;
    if (!textureBarrel.loadFromFile(Path_to_res + "barrel.png")) {
        return EXIT_FAILURE;
    }
    class object barrel;

    barrel.sprite.setTexture(textureBox);
    barrel.sprite.scale(0.07, 0.07);
    barrel.bound.setSize(sf::Vector2f(textureBox.getSize().x, textureBox.getSize().y));
    barrel.bound.setPosition(500, 100);
    barrel.moveable = true;
    barrel.bodyInit(world);


    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        return EXIT_FAILURE;
    }


    class player Player1(world, textureSans);


    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    //Крепления


/*
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(Player1.realBody, barrel.realBody, Player1.realBody->GetWorldCenter());
    b2RevoluteJoint* joint = (b2RevoluteJoint*)world.CreateJoint(&jointDef);
*/


    barrel.realBody->SetSleepingAllowed(false);

    while (window.isOpen()) {

        // check all the window's events that were triggered since the last iteration of the loop

        sf::Event event;

        while (window.pollEvent(event)) {

            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    //Прыжки

                    Player1.remainingJumpSteps = Player1.jumpHieght;
                }
                if (event.key.code == sf::Keyboard::G) {
                    //Grabbing stuff

                    if (Player1.cariedObject != nullptr) {
                        b2Body *box = Player1.cariedObject->realBody;

                        if (Player1.JointToHold == nullptr) {
                            b2RevoluteJointDef jointDef;

                            box->SetTransform(Player1.realBody->GetPosition(), 0);
                            box->SetFixedRotation(true);
                            jointDef.Initialize(Player1.realBody, box,
                                                Player1.realBody->GetWorldCenter());
                            b2RevoluteJoint *joint = (b2RevoluteJoint *) world.CreateJoint(&jointDef);
                            Player1.JointToHold = joint;
                        } else {
                            world.DestroyJoint(Player1.JointToHold);
                            box->SetFixedRotation(false);
                            Player1.throwObject(*box);

                            Player1.JointToHold = nullptr;
                            Player1.cariedObject = nullptr;
                        }
                    } else {

                    }


                }
            }
        }

        window.clear(sf::Color::Black);



        //обновление мира Box2D
        world.Step(timeStep, velocityIterations, positionIterations);
        Player1.update();
        Player1.move();

        barrel.update();

        floor.update();
        window.draw(Player1.sprite);
        //window.draw(Player1.bound);
        window.draw(floor.sprite);
        window.draw(barrel.sprite);

        window.setView(view1);
        view1.setCenter(Player1.bound.getPosition());


        window.display();
    }
#endif
    return 0;
}