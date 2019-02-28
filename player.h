//
// Created by leonid on 12.02.19.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#pragma once
#include "object.h"

class player: public object{
public:
    void print();

    object* cariedObject = nullptr;
    int remainingJumpSteps = 0;
    int jumpHieght = 6;
    float movementSpeed;
    int attackDamage;
    int counterWalking;
    int direction;//1 - up,2 - down, 3 - left, 4 - right
    player();
    void grabe();
    void update();
    player(b2World &world,sf::Texture&);
    void move();

private:

    int scale_factorX = 40;
    int scale_factorY = -40;
};


#endif //GAME_PLAYER_H
