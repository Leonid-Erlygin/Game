//
// Created by leonid on 12.02.19.
//

#ifndef GAME_WEAPON_H
#define GAME_WEAPON_H


#include "entity.h"

class weapon: public entity{
public:
    float movementSpeed;
    int attackDamage;
    int distance;
    bool isOn = 0;
    int direction;//1 - up,2 - down, 3 - left, 4 - right

    weapon();
    void update();

};

#endif //GAME_WEAPON_H
