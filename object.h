//
// Created by leonid on 18.02.19.
//

#include "entity.h"
#include "include/box2d/box2d.h"


#ifndef GAME_OBKECT_H
#define GAME_OBJECT_H
#pragma once

enum _entityCategory {
    PLAYER =          4,

};

enum WeaponType
{
	FireWeapon = 1,
	HandWeapon = 2,
	Grenade = 3,
	NotWeapon = 4
};


class object: public entity {
public:

   // b2FixtureDef fixture;//can be many of them, so this member is useless
    //b2BodyDef realBodyDef;//Можно не хранить объявления
    b2Body* realBody;
    WeaponType weapon_class = NotWeapon;
    bool isPlayer = false;
    float ancorPointShiftBodyAX = 0;
    float ancorPointShiftBodyBX = 0;
    float ancorPointShiftBodyAY= 0;
    float ancorPointShiftBodyBY= 0;
    //b2PolygonShape shape;//Это тоже можно удалить
    ~object();
    object();
    float dencity;
	int magicnumber = 0;
    bool movable;
    bool isBeingCaried = false;
    b2Body* isBeingCariedBy = nullptr;
    bool m_contacting = false;
    void startContact() { m_contacting = true; }
    void endContact() { m_contacting = false; }
    void virtual update();
    void bodyInit(b2World &world, float density = 5.0, int categoryBits = 2, int maskBits = 6);
    void print();

    int direction = 1;
    void flip(int dir);

private:
    float scale_factorX = 40;//1 м = 40 pxl
    float scale_factorY = -40;
};


#endif //GAME_BOX_H
