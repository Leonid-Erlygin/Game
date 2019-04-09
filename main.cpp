#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include <stdio.h>
#include <iostream>
#include "Box2D/Box2D.h"
#include "object.h"
#include "MyContactListener.h"
#include "virtualPlayer.h"
#include "weapon.h"
#include "handWeapon.h"
#include "grenade.h"


inline std::pair<float ,float > getMedian(player const &PlayerA, player const &PlayerB){
    sf::Vector2f m = PlayerA.bound.getPosition()+PlayerB.bound.getPosition();

    std::pair<float ,float > temp(m.x/2,m.y/2);
    return temp;
}
//in process...
inline sf::Vector2f zoom(player const&PlayerA, player const &PlayerB,sf::RenderWindow &window){
    sf::Vector2f c = PlayerB.bound.getPosition()-PlayerA.bound.getPosition();//proportions
    float sizeWin = window.getSize().x*window.getSize().x+window.getSize().y*window.getSize().y;
    if(c.x<0)c.x = -c.x;
    float k =(float)window.getSize().y/(float)window.getSize().x;
    if(c.y<0)c.y = -c.y;


    float d = c.x*c.x+c.y*c.y;//think about function
    int frac = 3;
    float capacity = 2.0f;
    if(sqrt(d)<sqrt(sizeWin)/frac){
        return sf::Vector2f(capacity*sqrt(sizeWin/(1+k*k))/frac,capacity*(sqrt(sizeWin/(1+k*k))/frac)*k);
    } else{


        return sf::Vector2f(capacity*(sqrt(d/(1+k*k))),capacity*(sqrt(d/(1+k*k)))*k);

    }
}

int main() {




//#if 1
    MyContactListener myContactListenerInstance;
    // Define the gravity vector.
    b2Vec2 gravity(0.0f, -15.81f);
    class World : public b2World {
        object *GetObjectList() {
        }
    };

    // Construct a world object, which will hold and simulate the rigid bodies.
    b2World world(gravity);


    world.SetContactListener(&myContactListenerInstance);//Нужно для детектирования столкновений
    /* ContactFilter filter;
     world.SetContactFilter(&filter);*/



    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");
    std::string Path_to_res = "/home/leonid/CLionProjects/Resourses/";
    std::string Path_to_duck = "/home/leonid/CLionProjects/Resourses/Duck Game Sprites/";
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
    sf::Texture textureWeapon;
    if (!textureWeapon.loadFromFile(Path_to_duck + "ak47.png")) {
        return EXIT_FAILURE;
    }

    sf::Texture textureBullet;
    if (!textureBullet.loadFromFile(Path_to_duck + "chainBullet.png")) {
        return EXIT_FAILURE;
    }

    sf::Texture textureBarrel;
    if (!textureBarrel.loadFromFile(Path_to_res + "barrel.png")) {
        return EXIT_FAILURE;
    }
    class object barrel;

    barrel.sprite.setTexture(textureBox);
    barrel.sprite.scale(0.2, 0.2);
    barrel.bound.setSize(sf::Vector2f(textureBox.getSize().x, textureBox.getSize().y));
    barrel.bound.setPosition(300, 100);
    barrel.moveable = true;
    barrel.bodyInit(world);


    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture textureBlade;
    if (!textureBlade.loadFromFile(Path_to_res + "blade1.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture textureFire;
    if (!textureFire.loadFromFile(Path_to_res + "fireTexture.png")) {
        return EXIT_FAILURE;
    }

    class player Player1(world, textureSans, 400, 350);
    class player Player2(world, textureSans, 200, 350);

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    floor.update();

    //  barrel.realBody->SetSleepingAllowed(false);

    sf::Texture texture_explosion;
    if (!texture_explosion.loadFromFile(Path_to_duck + "airFire.png")) {
        printf("FAILURE\n");
    }

    sf::Texture texture_grenade;
    if (!texture_grenade.loadFromFile(Path_to_duck + "grenade.png")) {
        printf("FAILURE\n");
    }


    class weapon weapon(world, window, textureWeapon, textureBullet, texture_explosion);
    class grenade grenade(world, window, texture_grenade, texture_explosion);
    class handWeapon bladeFire(world, textureBlade, textureFire);


    std::vector<sf::UdpSocket> sockets(2);

    sockets[0].setBlocking(false);
    sockets[1].setBlocking(false);

    std::pair<float ,float >median;
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed)
                window.close();

            Player2.checkEvents(event, world, 2);
            Player1.checkEvents(event, world, 1);//and send to socket;



        }

        window.clear(sf::Color::Black);



        world.Step(timeStep, velocityIterations, positionIterations);

        Player2.update();
        Player1.update();
        barrel.update();
        window.draw(floor.sprite);
        window.draw(barrel.sprite);
        window.setView(view1);


        median = getMedian(Player1,Player2);


        view1.setCenter(sf::Vector2f(median.first,median.second));

        view1.setSize(zoom(Player1,Player2,window));


        window.draw(Player1.sprite);
        window.draw(Player2.sprite);
        window.draw(bladeFire.sprite);
        bladeFire.update(window);

        weapon.weapon_update();
        grenade.grenade_update();

        window.display();


    }
//#endif
    return 0;
}
