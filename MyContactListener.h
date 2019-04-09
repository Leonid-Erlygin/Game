//
// Created by leonid on 26.02.19.
//

#ifndef GAME_MYCONTACTLISTENER_H
#define GAME_MYCONTACTLISTENER_H
#pragma once

#include "Box2D/Dynamics/b2WorldCallbacks.h"
#include "object.h"
#include "player.h"
#include "handWeapon.h"

class MyContactListener : public  b2ContactListener{
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    bool getContactInfo(b2Contact* contact, player*& playerEntity, object*& moveableObjectEntity);
    bool getContactInfo2(b2Contact *contact, handWeapon *&playerEntity, object *&moveableObjectEntity);
};


#endif //GAME_MYCONTACTLISTENER_H
