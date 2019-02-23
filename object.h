//
// Created by leonid on 18.02.19.
//

#include "entity.h"
#include "Box2D/Box2D.h"
#ifndef GAME_OBKECT_H
#define GAME_OBJECT_H
#pragma once

class object: public entity {
public:
    b2FixtureDef Fixture;
    b2BodyDef realBodyDef;
    b2Body* realBody;
    b2PolygonShape shape;
    object();
    void update();
    void bodyInit(b2World &world);
    void print();

private:
    float32 scale_factorX = 40;//1 м = 40 pxl
    float32 scale_factorY = -40;
};


#endif //GAME_BOX_H
