//
// Created by leonid on 26.02.19.
//

#include "MyContactListener.h"
#include "object.h"


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
    } else {

        auto entityA = static_cast<object *>( fixtureA->GetBody()->GetUserData());
        auto entityB = static_cast<player *>( fixtureB->GetBody()->GetUserData());
        if (!entityA->moveable ||
            (entityA->isBeingCariedBy != entityB->realBody && entityA->isBeingCariedBy != nullptr)) {
            return false;
        }
        playerEntity = entityB;
        moveableObjectEntity = entityA;
    }

    return true;
}

void MyContactListener::BeginContact(b2Contact *contact) {
    object *moveableEntity;
    player *playerEntity;

    if (getContactInfo(contact, playerEntity, moveableEntity)) {
        playerEntity->canTake = true;
        moveableEntity->isBeingCaried = true;
        moveableEntity->isBeingCariedBy = playerEntity->realBody;
        playerEntity->cariedObject = moveableEntity;
    }

}

void MyContactListener::EndContact(b2Contact *contact) {
    //check if fixture A was a ball
    object *moveableEntity;
    player *playerEntity;

    if (getContactInfo(contact, playerEntity, moveableEntity)) {
        moveableEntity->isBeingCaried = false;
        playerEntity->canTake = false;
        moveableEntity->isBeingCariedBy = nullptr;
    }
}