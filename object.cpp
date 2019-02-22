//
// Created by leonid on 18.02.19.
//

#include "object.h"


object::object() {

}
void object::bodyInit(b2World &world) {

    realBodyDef.position.Set(bound.getPosition().x/scale_factorX,bound.getPosition().y/scale_factorY);
    realBody = world.CreateBody(&realBodyDef);
    shape.SetAsBox((bound.getSize().x/(2*scale_factorX))*bound.getScale().x,(bound.getSize().y/(2*scale_factorX))*bound.getScale().y);
    realBody->CreateFixture(&shape,0.f);



}
void object::update() {


    b2Vec2 position = realBody->GetPosition();
    float32 angle = realBody->GetAngle();
    //bound.setRotation(-(angle*180)/(float32)(3.141));
    sprite.setRotation(-(angle*180)/(float32)(3.141));
    bound.setRotation(-(angle*180)/(float32)(3.141));
    bound.setPosition(scale_factorX*position.x-(bound.getSize().x/2)*bound.getScale().x,scale_factorY*position.y-((bound.getSize().y/2)*bound.getScale().y));

    sprite.setPosition(bound.getPosition());
}
