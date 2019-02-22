//
// Created by leonid on 15.02.19.
//

#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H


#include "entity.h"

class enemy : public entity{
public:
    float movementSpeed;
    int attackDamage;
    int counterWalking;
    int direction;//1 - up,2 - down, 3 - left, 4 - right
    int hp;
    bool alive;
    enemy();
    //void move();
    void update();
};


#endif //GAME_ENEMY_H
