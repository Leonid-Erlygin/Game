//
// Created by leonid on 26.02.19.
//

#include <iostream>
#include "MyContactListener.h"
#include "object.h"
#include "handWeapon.h"

#if 0
bool MyContactListener::getContactInfo(b2Contact *contact, player *&playerEntity, object *&moveableObjectEntity) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    //make sure only one of the fixtures was a sensor


    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();


    if (!(sensorA ^ sensorB))
        return false;
    //Один из объектов должен быть в покое(его никто не несёт)
  /*  if(fixtureA->GetBody()->GetUserData()== nullptr)
        return false;
    if(fixtureB->GetBody()->GetUserData()== nullptr)
        return false;
*/
    //один из объектов должен быть подвижным

    //Нужно подумать о разделении классов
    if (sensorA) {
        auto entityA = static_cast<player *>( fixtureA->GetBody()->GetUserData());
        auto entityB = static_cast<object *>( fixtureB->GetBody()->GetUserData());
        playerEntity = entityA;
        moveableObjectEntity = entityB;
        printf("InContact1\n");
        if(entityA->isPlayer) printf("InContact100\n");
        if (!entityA->isPlayer||entityB->isPlayer||!entityB->movable) {
            return false;
        }

    } else {

        auto entityA = static_cast<object *>( fixtureA->GetBody()->GetUserData());
        auto entityB = static_cast<player *>( fixtureB->GetBody()->GetUserData());
        if(entityB->isPlayer) printf("InContact100\n");
        if (!entityB->isPlayer||entityA->isPlayer||!entityA->movable) {
            return false;
        }
        playerEntity = entityB;
        moveableObjectEntity = entityA;

    }

    return true;
}
#endif

bool MyContactListener::getContactInfo2(b2Contact *contact, handWeapon *&handWeapon, object *&moveableObjectEntity) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    //make sure only one of the fixtures was a sensor

    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();
    if(fixtureA->GetBody()->GetUserData()== nullptr)
        return false;
    if(fixtureB->GetBody()->GetUserData()== nullptr)
        return false;


    if (!(sensorA ^ sensorB))
        return false;

    object *objectA;
    if (sensorA) {

        objectA = static_cast<object *>( fixtureA->GetBody()->GetUserData());
        if (objectA->weapon_class != HandWeapon)return false;

        handWeapon = static_cast<class handWeapon *>( fixtureA->GetBody()->GetUserData());
        moveableObjectEntity = static_cast<class object *>( fixtureB->GetBody()->GetUserData());
        //if (moveableObjectEntity->isPlayer)return false;

    } else {

        objectA = static_cast<object *>( fixtureB->GetBody()->GetUserData());
        if (objectA->weapon_class != HandWeapon)return false;

        handWeapon = static_cast<class handWeapon *>( fixtureB->GetBody()->GetUserData());
        moveableObjectEntity = static_cast<class object *>( fixtureA->GetBody()->GetUserData());
        //if (moveableObjectEntity->isPlayer)return false;


    }
    return true;
}


bool MyContactListener::getContactInfo(b2Contact *contact, player *&playerEntity, object *&moveableObjectEntity){
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();


    if(fixtureA->GetBody()->GetUserData()== nullptr)
        return false;
    if(fixtureB->GetBody()->GetUserData()== nullptr)
        return false;


    bool sensorA = fixtureA->IsSensor();
    bool sensorB = fixtureB->IsSensor();


    if (!(sensorA ^ sensorB))
        return false;


    auto entityA = static_cast<object *>( fixtureA->GetBody()->GetUserData());
    auto entityB = static_cast<object *>( fixtureB->GetBody()->GetUserData());

    if(entityA->isPlayer&&entityB->isPlayer)return false;

    if(entityA->isPlayer) {

        playerEntity = dynamic_cast<player *>( entityA);
        moveableObjectEntity = entityB;


    } else {

        if (entityB->isPlayer) {

            playerEntity = dynamic_cast<player *>( entityB);
            moveableObjectEntity = entityA;


        } else return false;
    }
    if(moveableObjectEntity->isBeingCaried)return false;
    return true;
}

void MyContactListener::BeginContact(b2Contact *contact) {
    object *moveableEntity;
    object *object1;
    handWeapon *handWeapon;
    player *playerEntity;



    if (getContactInfo(contact, playerEntity, moveableEntity)) {



        playerEntity->reachableObjects.insert( moveableEntity);

    }

    if (getContactInfo2(contact, handWeapon, object1)) {


        handWeapon->reachableObjects.insert(object1);
    }

}



void MyContactListener::EndContact(b2Contact *contact) {

    object *moveableEntity;
    object *object1;
    handWeapon *handWeapon;
    player *playerEntity;

    if (getContactInfo(contact, playerEntity, moveableEntity)) {

        playerEntity->reachableObjects.erase(moveableEntity);


    }

    if (getContactInfo2(contact, handWeapon, object1)) {

        handWeapon->reachableObjects.erase(object1);

    }

}
