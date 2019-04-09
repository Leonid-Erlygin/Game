//
// Created by leonid on 12.02.19.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#pragma once

#include "object.h"
#include "weapon.h"

#include <SFML/Network.hpp>
#include <set>

class player : public object {
public:
    void print();
    bool grab = false;
    int speed = 10;
    int strength = 5;
    std::set<object *>reachableObjects;
    object *cariedObject;
    int remainingJumpSteps = 1;
    int jumpHieght = 50;
    b2RevoluteJoint *JointToHold = nullptr;
    bool canTake = false;
    bool moveRight;
    bool moveLeft;
    bool moveUp = true;
    int direction;// -1 - left, 1 - right
   // player();

    bool is_on_ground();
    b2World& world;

    void grabe(b2World &);

    void update();
    void death(int x, int y);
    void sendEvent(sf::Event &,sf::UdpSocket&);



    void checkEvents(sf::Event &, b2World &,int player);

    player(b2World &world, sf::Texture &,int x,int y);

    void throwObject(b2Body &);

private:

    int scale_factorX = 40;
    int scale_factorY = -40;
};


#endif //GAME_PLAYER_H
