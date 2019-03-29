//
// Created by leonid on 26.02.19.
//

#include <iostream>
#include "MyContactListener.h"
#include "object.h"
#include "handWeapon.h"

bool MyContactListener::getContactInfo(b2Contact *contact, player *&playerEntity, object *&moveableObjectEntity) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    //make sure only one of the fixtures was a sensor

    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();


    if (!(sensorA ^ sensorB))
        return false;
    //Один из объектов должен быть в покое(его никто не несёт)


    //один из объектов должен быть подвижным

    //Нужно подомать о разделении классов
    if (sensorA) {
        auto entityA = static_cast<player *>( fixtureA->GetBody()->GetUserData());
        auto entityB = static_cast<object *>( fixtureB->GetBody()->GetUserData());
        playerEntity = entityA;
        moveableObjectEntity = entityB;
        if (entityA->grab) {
            return false;
        }
    } else {

        auto entityA = static_cast<object *>( fixtureA->GetBody()->GetUserData());
        auto entityB = static_cast<player *>( fixtureB->GetBody()->GetUserData());
        if (!entityA->moveable ||
            (entityA->isBeingCariedBy != entityB->realBody && entityA->isBeingCariedBy != nullptr)) {
            return false;
        }
        playerEntity = entityB;
        moveableObjectEntity = entityA;
        if (entityB->grab) {
            return false;
        }
    }

    return true;
}

bool MyContactListener::getContactInfo2(b2Contact *contact, handWeapon *&handWeapon, object *&moveableObjectEntity) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    //make sure only one of the fixtures was a sensor

    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();



    if (!(sensorA ^ sensorB))
        return false;

    object *objectA;
    if (sensorA) {

        objectA = static_cast<object *>( fixtureA->GetBody()->GetUserData());
        if (!objectA->isThisHandWeapon)return false;

        handWeapon = static_cast<class handWeapon *>( fixtureA->GetBody()->GetUserData());
        moveableObjectEntity = static_cast<class object *>( fixtureB->GetBody()->GetUserData());
        if (moveableObjectEntity->isPlayer)return false;

    } else {

        objectA = static_cast<object *>( fixtureB->GetBody()->GetUserData());
        if (!objectA->isThisHandWeapon)return false;

        handWeapon = static_cast<class handWeapon *>( fixtureB->GetBody()->GetUserData());
        moveableObjectEntity = static_cast<class object *>( fixtureA->GetBody()->GetUserData());
        if (moveableObjectEntity->isPlayer)return false;


    }
    return true;
}

void MyContactListener::BeginContact(b2Contact *contact) {
    object *moveableEntity;
    object *object1;
    handWeapon *handWeapon;
    player *playerEntity;




    if (getContactInfo(contact, playerEntity, moveableEntity)) {

        moveableEntity->isBeingCaried = true;
        moveableEntity->isBeingCariedBy = playerEntity->realBody;
        playerEntity->cariedObject = moveableEntity;


    }

    if (getContactInfo2(contact, handWeapon, object1)) {

        handWeapon->ContactObject = object1;
    }

}

void MyContactListener::EndContact(b2Contact *contact) {
    //check if fixture A was a ball
    object *moveableEntity;
    object *object1;
    handWeapon *handWeapon;
    player *playerEntity;

    if (getContactInfo(contact, playerEntity, moveableEntity)) {
        moveableEntity->isBeingCaried = false;
        moveableEntity->isBeingCariedBy = nullptr;
        playerEntity->cariedObject = nullptr;


    }

    if (getContactInfo2(contact, handWeapon, object1)) {

        handWeapon->ContactObject = nullptr;

    }

}