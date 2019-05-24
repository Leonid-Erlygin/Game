//
// Created by leonid on 03.03.19.
//

#include <iostream>
#include "virtualPlayer.h"
#include "grenade.h"
#include "handWeapon.h"

void virtualPlayer::update(std::vector<sf::UdpSocket> &socket,b2World& world,int x) {
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;
    int c = -1;
    int y = -1;
    if (socket[0].receive(packet,sender,port) == sf::Socket::Done)
    {
        sinhrCount++;
        packet >> c;
    }
    ;

    sf::Event event;

    float32 x1 = 0;
    float32 y1 = 0;
    if (c == sf::Event::EventType::KeyPressed || c == sf::Event::EventType::KeyReleased) {
        packet >> y;
        packet>>x1;
        packet>>y1;
        realBody->SetTransform(b2Vec2(x1,y1),0);

        event.type = sf::Event::EventType(c);
        event.key.code = sf::Keyboard::Key(y);

    } else{
        c = -1;
    }

    if (c!=-1&&event.type == sf::Event::KeyPressed) {

        if(event.key.code == sf::Keyboard::Space){
            if (grab) {
                if (cariedObject->weapon_class != NotWeapon) {

                    try {

                        if (cariedObject->weapon_class == Grenade) {
                            auto weapon = dynamic_cast<class grenade *>(cariedObject);
                            weapon->strike();
                        } else if (cariedObject->weapon_class == FireWeapon) {
                            auto weapon = dynamic_cast<class weapon *>(cariedObject);
                            weapon->strike();
                        } else if (cariedObject->weapon_class == HandWeapon) {
                            auto weapon = dynamic_cast<class handWeapon *>(cariedObject);
                            weapon->strike();
                        }

                    }
                    catch (const std::bad_cast &e) {
                        std::cout << e.what() << std::endl;
                        std::cout << "Этот объект не является объектом типа Weapon\n" << std::endl;
                    }

                }
            }
        }

        if (event.key.code == sf::Keyboard::Right) {
            moveRight = true;

        }


        if (event.key.code == sf::Keyboard::Left) {
            moveLeft = true;
        }

        if (event.key.code == sf::Keyboard::Up) {
            //Прыжки


                if (is_on_ground()) {
                    realBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpHeight), true);
                }


        }

        if (event.key.code == sf::Keyboard::G) {

            grabe(world);


        }
    }


    if (c!=-1&&event.type == sf::Event::KeyReleased) {

        if (event.key.code == sf::Keyboard::Right) {
            moveRight = false;
            //Send Message to virtualPlayers
        }
        if (event.key.code == sf::Keyboard::Left) {
            moveLeft = false;
        }
    }

    player::update();
    if ((x1!=0||y1!=0)){
        realBody->SetTransform(b2Vec2(x1,y1),0);
        sinhrCount = 0;
    }
}

virtualPlayer::virtualPlayer(b2World &world, sf::Texture &Player_texture,sf::SoundBuffer&s, int x, int y) : player(world, Player_texture,
                                                                                                 s,x, y) {
}