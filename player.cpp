//
// Created by leonid on 12.02.19.
//

#include "player.h"


player::player() {//Тут все заточено под конкретный спрайт, нужно исправить
    bound.setPosition(400,200);
    sprite.scale(0.25,0.25);

    bound.setSize(sf::Vector2f(235,310));
    sprite.setTextureRect(sf::IntRect(30,0,235,310));
    bound.scale(0.25,0.25);
    bound.setFillColor(sf::Color::Blue);
    counterWalking = 0;
    movementSpeed = 10;
}
player::player(b2World &world)//игрока не бывает без физики
{

    sprite.setTextureRect(sf::IntRect(30,0,235,310));
    bound.setPosition(400,0);
    sprite.scale(0.25,0.25);

    bound.setSize(sf::Vector2f(235,310));

    bound.scale(0.25,0.25);
    bound.setFillColor(sf::Color::Blue);
    counterWalking = 0;
    movementSpeed = 10;


    realBodyDef.type = b2_dynamicBody;
    realBodyDef.position.Set(bound.getPosition().x/scale_factorX, bound.getPosition().y/scale_factorY);
    realBody = world.CreateBody(&realBodyDef);
    shape.SetAsBox((bound.getSize().x/(float32)(2*scale_factorX))*bound.getScale().x, (bound.getSize().y/(float32)(2*scale_factorX))*bound.getScale().y);
    Fixture.shape = &shape;
    Fixture.density = 1.0f;

    // Override the default friction.
    Fixture.friction = 0.3f;

    // Add the shape to the body.
    realBody->CreateFixture(&Fixture);
}
void physicsDef(){

}

void player::move() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(0,2), true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(0,-1), true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(-1,0), true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        realBody->ApplyLinearImpulseToCenter(b2Vec2(1,0), true);
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