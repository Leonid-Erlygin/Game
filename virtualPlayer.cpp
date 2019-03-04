//
// Created by leonid on 03.03.19.
//

#include "virtualPlayer.h"


void virtualPlayer::update(sf::TcpListener &listener, sf::TcpSocket &socket,b2World& world) {
    sf::Packet packet;
    socket.receive(packet);
    sf::Event event;
    int x;
    int y;
    packet >> x;

    if (x == sf::Event::EventType::KeyPressed || x == sf::Event::EventType::KeyReleased) {
        packet >> y;
        event.type = sf::Event::EventType(x);
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

            remainingJumpSteps = jumpHieght;


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

}

virtualPlayer::virtualPlayer(b2World &world, sf::Texture &Player_texture, int x, int y) : player(world, Player_texture,
                                                                                                 x, y) {

}