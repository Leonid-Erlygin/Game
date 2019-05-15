#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unistd.h>
#include <vector>
#include "player.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Box2D/Box2D.h"
#include "object.h"
#include "MyContactListener.h"
#include "virtualPlayer.h"
#include "weapon.h"
#include "handWeapon.h"
#include "grenade.h"
#include "Map/level.h"
#include "GameCore.h"

void addGid(){

    std::ifstream inFile;
    std::ofstream outFile;
    std::string s;
    outFile.open("/home/leonid/CLionProjects/TestMap/TestEdited.tmx");
    inFile.open("/home/leonid/CLionProjects/TestMap/Test2.tmx");
    while (getline(inFile,s)){
        if (s.find("<tile")!=-1){
            if(s.find("gid")==-1){
                s =  "<tile gid=\"0\"/>";
            }
        }
        s +="\n";
        outFile<<s;

    }
    inFile.close();
    outFile.close();
}



int main() {
#if 0
    int x;
    std::cin>>x;
    std::vector<sf::UdpSocket> socket(2);
    //socket[0].setBlocking(false);
   // socket[1].setBlocking(false);

    if(x==1){
        sf::IpAddress recipient = ;
        unsigned short port;
        port = 54001;
        sf::Packet packet;
        packet<<1;
        socket[1].send(packet, recipient, port);

    } else{

        if (socket[0].bind(54001) != sf::Socket::Done)
        {
            exit(0);
        }
        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short port;
        int c = -1;

            if (socket[0].receive(packet, sender, port) == sf::Socket::Done) {
                packet >> c;
                printf("%d\n",c);
            }

    }
#endif

#if 1
    std::vector<sf::UdpSocket> socket(2);
    socket[0].setBlocking(false);
    socket[1].setBlocking(false);

#if 1
    int x;
    //std::cin>>x;
    x = 1;
    if(x==1){

        if (socket[0].bind(54000) != sf::Socket::Done)
        {
            exit(0);
        }

    } else{

        if (socket[0].bind(54001) != sf::Socket::Done)
        {
            exit(0);
        }
    }
#endif
    MyContactListener myContactListenerInstance;
    b2Vec2 gravity(0.0f, -15.81f);
    b2World world(gravity);
    sf::Music music;

    if (!music.openFromFile("/home/leonid/CLionProjects/Game/SFX/Megalovania.ogg"))
    {
        printf("failure when loading music\n");
        return 1;
    }
    music.setVolume(22.f);
   // music.play();

    world.SetContactListener(&myContactListenerInstance);
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);



    GameCore game(window,world,x);
    //game.runMenu();
    game.initLvl();
    game.loadTextures();
    game.loadSoundBuffers();
    game.createMovableObjects();

    game.runLevel(socket);

#endif
    return 0;
}
