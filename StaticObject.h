//
// Created by leonid on 13.04.19.
//

#ifndef GAME_STATICOBJECT_H
#define GAME_STATICOBJECT_H


#include "object.h"
#include "Map/level.h"
class StaticObject: public object {
public:
    StaticObject(b2World& world,Object &);

};


#endif //GAME_STATICOBJECT_H
