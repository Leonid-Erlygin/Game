//
// Created by leonid on 03.03.19.
//

#include <iostream>
#include "virtualPlayer.h"
#include "grenade.h"
#include "handWeapon.h"

void
virtualPlayer::update(std::vector<sf::UdpSocket> &socket, b2World &world, bool update_without_package,
                      int c, int ev, int key, float x, float y, float vx, float vy) {

//    sf::Packet packet;
//    sf::IpAddress sender;
//    unsigned short port;
//    int c = -2;
//    int key_code = -1;
//    if (socket[0].receive(packet, sender, port) == sf::Socket::Done) {
//        packet >> c;
//    };
    if (update_without_package) {
        player::update(socket, -1,
                       false); // As virtualPlayer class inherits the player class we can just use this function

    } else {
        if (c == -1) {
            realBody->SetTransform(b2Vec2(x, y), 0);
            realBody->SetLinearVelocity(b2Vec2(vx, vy));
            player::update(socket,-1,
                           false); // As virtualPlayer class inherits the player class we can just use this function
        } else if (c == -2) {
            sf::Event event;
            realBody->SetTransform(b2Vec2(x, y), 0);
            realBody->SetLinearVelocity(b2Vec2(vx, vy));
            if (ev == sf::Event::EventType::KeyPressed || ev == sf::Event::EventType::KeyReleased) {
                event.type = sf::Event::EventType(ev);
                event.key.code = sf::Keyboard::Key(key);

            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
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


            if (event.type == sf::Event::KeyReleased) {

                if (event.key.code == sf::Keyboard::Right) {
                    moveRight = false;
                    //Send Message to virtualPlayers
                }
                if (event.key.code == sf::Keyboard::Left) {
                    moveLeft = false;
                }
            }
            player::update(socket, -1,
                           false); // As virtualPlayer class inherits the player class we can just use this function
        }
    }

}

virtualPlayer::virtualPlayer(b2World &world, sf::Texture &Player_texture, sf::SoundBuffer &s, int x, int y) : player(
        world, Player_texture,
        s, x, y) {
}