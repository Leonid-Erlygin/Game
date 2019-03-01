//
// Created by leonid on 18.02.19.
//

#include "entity.h"
#include "Box2D/Box2D.h"


#ifndef GAME_OBKECT_H
#define GAME_OBJECT_H
#pragma once

enum _entityCategory {
    PLAYER =          4,
};


class object: public entity {
public:

   // b2FixtureDef fixture;//can be many of them, so this member is useless
    //b2BodyDef realBodyDef;//Можно не хранить объявления
    b2Body* realBody;
    //b2PolygonShape shape;//Это тоже можно удалить
    ~object();
    object();
    bool moveable;
    bool isBeingCaried = false;
    b2Body* isBeingCariedBy = nullptr;
    bool m_contacting = false;
    void startContact() { m_contacting = true; }
    void endContact() { m_contacting = false; }
    void virtual update();
    void bodyInit(b2World &world);
    void print();

private:
    float32 scale_factorX = 40;//1 м = 40 pxl
    float32 scale_factorY = -40;
};


#endif //GAME_BOX_H
