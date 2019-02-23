#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include "weapon.h"
#include "enemy.h"
#include <stdio.h>
#include <iostream>
#include "Box2D/Box2D.h"
#include "object.h"

int main() {


    // Define the gravity vector.
    b2Vec2 gravity(0.0f, -9.81f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    b2World world(gravity);

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
    floor.bound.setPosition(400,500);
    floor.sprite.setTextureRect(sf::IntRect(0,65,96,79-65));




    //std::cout<<floor.bound.getSize().x<<" "<<floor.bound.getSize().y<<'\n';
    //std::cout<<floor.sprite.getTextureRect().width<<" "<<floor.sprite.getTextureRect().height<<'\n';

    floor.sprite.scale(20,3);//Не изменяет значения границ, нужно домножать на этот коэфицент при переводе в метры

    //std::cout<<floor.bound.getSize().x<<" "<<floor.bound.getSize().y<<'\n';
    //std::cout<<floor.sprite.getTextureRect().width<<" "<<floor.sprite.getTextureRect().height;

    floor.bodyInit(world);
    floor.realBody->CreateFixture(&floor.shape,0.f);
/*
    object wall1;

    wall1.sprite.setTexture(textureCastle);
    wall1.sprite.setTextureRect(sf::IntRect(0,65,96,14));
    wall1.bound.setPosition(0,500);
    wall1.sprite.scale(20,3);
    wall1.sprite.setRotation(90);

    wall1.bodyInit(world);

    wall1.realBody->CreateFixture(&wall1.shape,0.f);

*//*
    object wall2;

    wall2.sprite.setTexture(textureCastle);
    wall2.sprite.setTextureRect(sf::IntRect(0,65,96,14));
    wall2.bound.setPosition(1000,500);
    wall2.sprite.scale(20,3);
    wall2.sprite.setRotation(90);

    wall2.bodyInit(world);
    wall2.realBody->CreateFixture(&wall2.shape,0.f);
*/
    sf::View view1;
    view1.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));
    view1.setCenter(sf::Vector2f(view1.getSize().x/2,view1.getSize().y/2));
    window.setView(view1);


    sf::Texture textureBox;
    if(!textureBox.loadFromFile(Path_to_res+"box1.png")){
        return EXIT_FAILURE;
    }

    sf::Texture textureBarrel;
    if(!textureBarrel.loadFromFile(Path_to_res+"barrel.png")){
        return EXIT_FAILURE;
    }
    class object barrel;
    barrel.sprite.setTexture(textureBox);
    barrel.sprite.scale(0.25,0.25);
    barrel.bound.setSize(sf::Vector2f(textureBox.getSize().x,textureBox.getSize().y));
    barrel.bound.scale(0.25,0.25);
    barrel.bound.setPosition(500,100);


    // Define the dynamic body. We set its position and call the body factory.
    barrel.realBodyDef.type = b2_dynamicBody;
    barrel.bodyInit(world);

    // Define another box shape for our dynamic body.
    //b2PolygonShape dynamicBox;

    // Define the dynamic body fixture.

    barrel.Fixture.shape = &barrel.shape;

    // Set the box density to be non-zero, so it will be dynamic.
    barrel.Fixture.density = 1.0f;

    // Override the default friction.
    barrel.Fixture.friction = 0.3f;

    // Add the shape to the body.
    barrel.realBody->CreateFixture(&barrel.Fixture);

/*

    class object box;
    box.sprite.setTexture(textureBox);
    box.sprite.setScale(0.25,0.25);
    box.bound.setSize(sf::Vector2f(textureBox.getSize().x,textureBox.getSize().y));
    box.bound.setScale(0.25,0.25);
    box.bound.setPosition(0,400);
*/

    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        return EXIT_FAILURE;
    }


    class player Player1(world);

    Player1.sprite.setTexture(textureSans);




    std::vector<weapon>::const_iterator iter;
    std::vector<weapon> weaponArray;
    weapon weapon1;


    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    while (window.isOpen()) {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {

            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        //столкновения

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
        Player1.update();//Теперь спрайт привязан к прямоугольнику


        Player1.move();

        //обновление мира Box2D
        world.Step(timeStep, velocityIterations, positionIterations);


       // barrel.sprite.setRotation(-angle);
        barrel.update();


        floor.update();

        //window.draw(wall1.sprite);
        //window.draw(wall2.sprite);
        window.draw(floor.sprite);
        window.draw(barrel.sprite);

        window.setView(view1);
        view1.setCenter(Player1.bound.getPosition());
        window.draw(Player1.bound);
        window.draw(Player1.sprite);

        window.display();
    }
#endif
    return 0;
}