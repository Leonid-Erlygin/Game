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




}
void object::update() {



    //bound.setRotation(-(angle*180)/(float32)(3.141));
    sprite.setRotation(-(realBody->GetAngle()*180)/(float32)(3.141));
    bound.setRotation(-(realBody->GetAngle()*180)/(float32)(3.141));
    bound.setPosition(scale_factorX*realBody->GetPosition().x+shape.m_vertices[0].x*scale_factorX,scale_factorY*realBody->GetPosition().y+shape.m_vertices[0].y*scale_factorX);

    sprite.setPosition(bound.getPosition());
    //print();
}

void object::print() {
    printf("Bound Position x:%f ,Bound Position y:%f\n Body position x:%f,Body position y:%f\n",bound.getPosition().x,bound.getPosition().y,
            realBody->GetPosition().x*scale_factorX,realBody->GetPosition().y*scale_factorY);
    printf("BoxParam width:%f,height%f\n",shape.m_vertices[1].x*scale_factorX-shape.m_vertices[0].x*scale_factorX,shape.m_vertices[2].y*scale_factorX-shape.m_vertices[1].y*scale_factorX);
    printf("Bound param width:%f, height:%f\n",bound.getSize().x*bound.getScale().x,bound.getSize().y*bound.getScale().y);
    printf("Sprite param width:%f, height:%f\n",sprite.getTextureRect().width*sprite.getScale().x,sprite.getTextureRect().height*sprite.getScale().y);
}
