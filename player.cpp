//
// Created by leonid on 12.02.19.
//

#include "player.h"


player::player() {//Тут все заточено под конкретный спрайт, нужно исправить
}
player::player(b2World &world,sf::Texture &Player_texture)
{

    moveable = true;


    sprite.setTexture(Player_texture);
    sprite.setTextureRect(sf::IntRect(30,0,235,310));
    bound.setPosition(400,0);
    sprite.scale(0.25,0.25);

    bound.setSize(sf::Vector2f(235,310));

    bound.scale(0.25,0.25);
    bound.setFillColor(sf::Color::Blue);
    counterWalking = 0;
    movementSpeed = 10;

    b2BodyDef realBodyDef;
    b2PolygonShape shape;
    b2FixtureDef fixture;

    realBodyDef.type = b2_dynamicBody;
    realBodyDef.position.Set(bound.getPosition().x/scale_factorX, bound.getPosition().y/scale_factorY);
    realBody = world.CreateBody(&realBodyDef);
    shape.SetAsBox((bound.getSize().x/(float32)(2*scale_factorX))*bound.getScale().x, (bound.getSize().y/(float32)(2*scale_factorX))*bound.getScale().y);

    fixture.shape = &shape;
    fixture.density = 5.0f;


    fixture.filter.categoryBits = PLAYER;//I'm a...
    fixture.filter.maskBits = 1 ; //I will collide with...
                                    //if((categoryA & maskB)!=0 && (categoryB & maskA)!=0){
                                    //      collision
                                    //}

    fixture.friction = 0.3f;

    b2PolygonShape shape2;
    shape2.SetAsBox(shape.m_normals[3].x, shape.m_normals[3].y);
    b2FixtureDef fixture2;
    fixture2.shape = &shape2;
    fixture2.density = 0;
    fixture2.isSensor = true;
    fixture2.filter.categoryBits = 6;
    fixture2.filter.categoryBits = 2;

    // Add the shape to the body.

    realBody->CreateFixture(&fixture);
    realBody->CreateFixture(&fixture2);//sensor
    realBody->SetFixedRotation(true);
    realBody->SetUserData( this );
}
void physicsDef(){

}
#if 1
void player::update() {
    object::update();
    if ( remainingJumpSteps > 0 ) {
        realBody->ApplyForce( b2Vec2(0,1500), realBody->GetWorldCenter(),true );
        remainingJumpSteps--;
    }
}

#endif
void player::move() {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(0,-3), true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(-3,0), true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(3,0), true);
    }

    counterWalking++;
    if(counterWalking==2){
        counterWalking = 0;
    }
}



void player::print(){
    printf("Bound height:%f Bound width:%f\nBox height:%f Box width:%f\n",bound.getSize().y*bound.getScale().y,
            bound.getSize().x*bound.getScale().x,(bound.getSize().y/(float32)(2*scale_factorX))*bound.getScale().y,
           (bound.getSize().x/(float32)(2*scale_factorX))*bound.getScale().x);
}



void player::grabe() {

}