//
// Created by leonid on 12.02.19.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#pragma once

#include "object.h"
#include "weapon.h"

#include <SFML/Network.hpp>
class player : public object {
public:
    void print();

    int strength = 5;
    object *cariedObject = nullptr;
    int remainingJumpSteps = 0;
    int jumpHieght = 6;
    b2RevoluteJoint *JointToHold = nullptr;
    bool canTake = false;

    weapon * cariedWeapon;
    bool moveRight;
    bool moveLeft;
    int direction;// -1 - left, 1 - right
    player();

    void grabe(b2World &);

    void virtual update();

    void sendEvent(sf::Event &,sf::UdpSocket&);



    void checkEvents(sf::Event &, b2World &,sf::TcpSocket &);

    player(b2World &world, sf::Texture &,int x,int y);

    void throwObject(b2Body &);

private:

    int scale_factorX = 40;
    int scale_factorY = -40;
};


#endif //GAME_PLAYER_H
