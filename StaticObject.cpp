//
// Created by leonid on 13.04.19.
//

#include "StaticObject.h"


StaticObject::StaticObject(b2World &world,Object& Object) {
        type  = StaticObj;
        int m = Object.sprite.getTextureRect().width;
        int n = Object.sprite.getTextureRect().height;
        bound.setSize(sf::Vector2f(m,n));
        //bound.setScale(sprite.getScale());
        //bound.setRotation(sprite.getRotation());
        int x = Object.rect.left;
        int y = Object.rect.top;
        bound.setPosition(x,y);

        b2BodyDef realBodyDef;
        b2PolygonShape shape;

        realBodyDef.position.Set(bound.getPosition().x/scale_factorX,bound.getPosition().y/scale_factorY);
        realBodyDef.type = b2_staticBody;

        shape.SetAsBox((bound.getSize().x/(2*scale_factorX))*bound.getScale().x,(bound.getSize().y/(2*scale_factorX))*bound.getScale().y);
        realBody = world.CreateBody(&realBodyDef);
        realBody->CreateFixture(&shape, 0.f);
        realBody->SetUserData( this );

}