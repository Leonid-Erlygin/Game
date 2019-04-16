//
// Created by leonid on 03.03.19.
//

#include "virtualPlayer.h"


void virtualPlayer::update(std::vector<sf::UdpSocket> &socket,b2World& world,int x) {
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;
    socket[x].receive(packet,sender,port);

    sf::Event event;
    int c;
    int y;
    packet >> c;

    if (c == sf::Event::EventType::KeyPressed || c == sf::Event::EventType::KeyReleased) {
        packet >> y;
        event.type = sf::Event::EventType(c);
        event.key.code = sf::Keyboard::Key(y);

    }

    if (event.type == sf::Event::KeyPressed) {


        if (event.key.code == sf::Keyboard::Right) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::Left) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::Up) {
            //Прыжки

            remainingJumpSteps = jumpHeight;


        }

        if (event.key.code == sf::Keyboard::G) {

            grabe(world);


        }
    }


    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Right) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::Left) {
            moveLeft = false;
        }
    }
    player::update();

}

virtualPlayer::virtualPlayer(b2World &world, sf::Texture &Player_texture, int x, int y) : player(world, Player_texture,
                                                                                                 x, y) {

}