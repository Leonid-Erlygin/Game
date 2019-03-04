#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include <stdio.h>
#include <iostream>
#include "Box2D/Box2D.h"
#include "object.h"
#include "MyContactListener.h"
#include "virtualPlayer.h"
#include "weapon.h"
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


    class player Player1(world, textureSans, 400, 0);

    std::vector<player> MultiPlayer;


    class virtualPlayer Player2(world, textureSans, 200, 0);


    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    floor.update();

    barrel.realBody->SetSleepingAllowed(false);

    sf::TcpSocket socket;
   // sf::Socket::Status status = socket.connect("127.0.1.1", 2000);
/*
    if (status != sf::Socket::Done) {
        printf("SocketConnectionError\n");
        exit(0);
    }
    sf::TcpListener listener;

    // bind the listener to a port
    if (listener.listen(2000) != sf::Socket::Done) {
        printf("ListenerConnectionError\n");
    }

    // accept a new connection
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done) {
        printf("ClientSocketConnectionError\n");
    }
*/
    class weapon weapon(world);


    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed)
                window.close();
            if (event.key.code == sf::Keyboard::Space){
                weapon.strike();
            }

            Player1.checkEvents(event, world, socket);//and send to socket;



        }

        window.clear(sf::Color::Black);


        world.Step(timeStep, velocityIterations, positionIterations);


        Player1.update();
        //Player2.update(listener,client,world);
        barrel.update();

        window.draw(Player2.sprite);

        window.draw(Player1.sprite);
        window.draw(floor.sprite);
        window.draw(barrel.sprite);
        window.setView(view1);
        weapon.weapon_update();
        view1.setCenter(Player1.bound.getPosition());
        window.draw(weapon.sprite);
        window.display();
    }
#endif
    return 0;
}