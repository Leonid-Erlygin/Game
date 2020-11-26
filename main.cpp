#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "include/box2d/box2d.h"
#include "object.h"
#include "MyContactListener.h"
#include "virtualPlayer.h"
#include "weapon.h"
#include "handWeapon.h"
#include "grenade.h"
#include "Map/level.h"
#include "GameCore.h"

void addGid() {

    std::ifstream inFile;
    std::ofstream outFile;
    std::string s;
    outFile.open("/home/leonid/CLionProjects/TestMap/TestEdited.tmx");
    inFile.open("/home/leonid/CLionProjects/TestMap/Test2.tmx");
    while (getline(inFile, s)) {
        if (s.find("<tile") != -1) {
            if (s.find("gid") == -1) {
                s = "<tile gid=\"0\"/>";
            }
        }
        s += "\n";
        outFile << s;

    }
    inFile.close();
    outFile.close();
}


int main() {
#if 0
    int x;
    printf("%d", x);

#endif

#if 1
    std::vector<sf::UdpSocket> socket(2);
    socket[0].setBlocking(false);
    socket[1].setBlocking(false);

#if 1
    int number_of_players = 3;
    int x;
    std::cin >> x;
    int base = 54001;

    if (socket[0].bind(base - x) != sf::Socket::Done) {
            exit(0);
    }


    //now sockets are ready to receive data on that port
#endif
    MyContactListener myContactListenerInstance;
    b2Vec2 gravity(0.0f, -15.81f);
    b2World world(gravity);
    sf::Music music;

    if (!music.openFromFile("/home/leonid/CLionProjects/Game/Resourses/Sounds/Megalovania.ogg")) {
        printf("failure when loading music\n");
        return 1;
    }
    music.setVolume(22.f);
    //music.play();

    world.SetContactListener(&myContactListenerInstance);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");
    window.setFramerateLimit(60);


    GameCore game(window, world, x, 20,number_of_players);
    game.runMenu();
    if (game.isLocal) {
        game.initLvl("GrassLand");

    } else{
        game.initLvl("GrassLand");
    }

    game.loadTextures();
    game.loadSoundBuffers();


    game.runLevel(socket);


    return 0;
}

#endif