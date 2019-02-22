//
// Created by leonid on 15.02.19.
//

#include "enemy.h"


enemy::enemy()
{
    bound.setSize(sf::Vector2f(235,310));
    bound.setPosition(400,200);
    sprite.scale(0.25,0.25);
    sprite.setTextureRect(sf::IntRect(30,0,235,310));
    bound.scale(0.05,0.05);
    bound.setFillColor(sf::Color::Blue);
    counterWalking = 0;
    movementSpeed = 0.05;
    int hp=3;
    alive = true;
}

void enemy::update() {
    sprite.setPosition(bound.getPosition());
}
