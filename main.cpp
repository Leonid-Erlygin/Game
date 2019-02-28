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

#if 1
    int counter;
    int counter1;
    float32 scaleX = 40;
    float32 scaleY = -40;
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::string Path_to_res = "/home/leonid/CLionProjects/Game/Resourses/";
    std::string Path_to_duck = "/home/leonid/CLionProjects/Game/Resourses/Duck Game Sprites/";
    sf::Texture textureCastle;
    if (!textureCastle.loadFromFile(Path_to_duck + "castle.png")) {
        return EXIT_FAILURE;
    }
    window.setFramerateLimit(60);
    //Viewm

    object floor;

    floor.sprite.setTexture(textureCastle);
    floor.bound.setPosition(400, 500);
    floor.sprite.setTextureRect(sf::IntRect(0, 65, 96, 79 - 65));

    floor.moveable = false;


    //std::cout<<floor.bound.getSize().x<<" "<<floor.bound.getSize().y<<'\n';
    //std::cout<<floor.sprite.getTextureRect().width<<" "<<floor.sprite.getTextureRect().height<<'\n';

    floor.sprite.scale(20, 3);//Не изменяет значения границ, нужно домножать на этот коэфицент при переводе в метры

    //std::cout<<floor.bound.getSize().x<<" "<<floor.bound.getSize().y<<'\n';
    //std::cout<<floor.sprite.getTextureRect().width<<" "<<floor.sprite.getTextureRect().height;


    floor.bodyInit(world);


    //floor.realBody->CreateFixture(&floor.shape, 0.f);

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
    // Define the dynamic body. We set its position and call the body factory.


    /*
    barrel.realBodyDef.type = b2_dynamicBody;


    // Define another box shape for our dynamic body.
    //b2PolygonShape dynamicBox;

    // Define the dynamic body fixture.

    barrel.fixture.shape = &barrel.shape;

    // Set the box density to be non-zero, so it will be dynamic.
    barrel.fixture.density = 1.0f;

    // Override the default friction.
    barrel.fixture.friction = 0.3f;

    barrel.fixture.filter.categoryBits = 2;//
    barrel.fixture.filter.maskBits = 6|1|4;
    // Add the shape to the body.
    barrel.realBody->CreateFixture(&barrel.fixture);
*/
    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        return EXIT_FAILURE;
    }


    class player Player1(world, textureSans);




    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    //Крепления


/*
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(Player1.realBody, barrel.realBody, Player1.realBody->GetWorldCenter());
    b2RevoluteJoint* joint = (b2RevoluteJoint*)world.CreateJoint(&jointDef);
*/




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

                    if(Player1.cariedObject!= nullptr){
                        printf("Несу\n");
                    } else{
                        printf("Не несу\n");
                    }


                }
            }
        }

        window.clear(sf::Color::Black);


        //столкновения
        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

            weapon1.bound.setPosition(Player1.bound.getPosition());
            //Теперь объект оружия рядом с человеком
            weapon1.direction = Player1.direction;
            weaponArray.push_back(weapon1);

        }
        counter1 = 0;
        for (iter = weaponArray.begin(); iter != weaponArray.end(); iter++) {
            if (weaponArray[counter1].bound.getPosition().y>600||weaponArray[counter1].bound.getPosition().y<0
            ||weaponArray[counter1].bound.getPosition().x<0||weaponArray[counter1].bound.getPosition().x>800) {
                weaponArray.erase(iter);
                break;//Why?
            }
            counter1++;
        }

        //draw projectile


        counter = 0;
        for (iter = weaponArray.begin(); iter != weaponArray.end(); iter++) {
            weaponArray[counter].update();
            window.draw(weaponArray[counter].bound);
            counter++;
        }
         */
        //Теперь спрайт привязан к прямоугольнику


        //обновление мира Box2D
        world.Step(timeStep, velocityIterations, positionIterations);
        Player1.update();
        Player1.move();

        barrel.update();


        floor.update();
        window.draw(Player1.sprite);
        //window.draw(wall1.sprite);
        //window.draw(wall2.sprite);
        window.draw(floor.sprite);
        window.draw(barrel.sprite);

        window.setView(view1);
        view1.setCenter(Player1.bound.getPosition());


        window.display();
    }
#endif
    return 0;
}