//
// Created by leonid on 18.02.19.
//

#include "object.h"

;

object::object()=default;


object::~object()=default;


void object::bodyInit(b2World &world) {
    bound.setSize(sf::Vector2f(sprite.getTextureRect().width,sprite.getTextureRect().height));
    bound.setScale(sprite.getScale());
    bound.setRotation(sprite.getRotation());

    b2BodyDef realBodyDef;
    b2PolygonShape shape;
    b2FixtureDef fixture;


    realBodyDef.position.Set(bound.getPosition().x/scale_factorX,bound.getPosition().y/scale_factorY);



    shape.SetAsBox((bound.getSize().x/(2*scale_factorX))*bound.getScale().x,(bound.getSize().y/(2*scale_factorX))*bound.getScale().y);


    if(moveable){

        fixture.shape = &shape;
        realBodyDef.type = b2_dynamicBody;
        // Set the box density to be non-zero, so it will be dynamic.
        fixture.density = 5.0f;

        // Override the default friction.
        fixture.friction = 0.3f;

        fixture.filter.categoryBits = 2;//
        fixture.filter.maskBits = 6|1|4;
        fixture.userData = (void *)2;
        realBody = world.CreateBody(&realBodyDef);
        realBody->CreateFixture(&fixture);


    } else{
        realBody = world.CreateBody(&realBodyDef);
        realBody->CreateFixture(&shape, 0.f);

    }
    realBody->SetUserData( this );


}
void object:: update() {

    bound.setOrigin(bound.getSize().x/2,bound.getSize().y/2);
    sprite.setOrigin(bound.getSize().x/2,bound.getSize().y/2);
    sprite.setRotation(-(realBody->GetAngle()*180)/(3.14159265f));
    bound.setRotation(-(realBody->GetAngle()*180)/(3.14159265f));
    bound.setPosition(scale_factorX*realBody->GetPosition().x/*+shape.m_vertices[0].x*scale_factorX*/,scale_factorY*realBody->GetPosition().y/*shape.m_vertices[0].y*scale_factorX*/);
    sprite.setPosition(bound.getPosition());

}
