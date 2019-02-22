//
// Created by leonid on 12.02.19.
//

#include "weapon.h"

weapon::weapon() {
    bound.setSize(sf::Vector2f(235, 310));
    bound.setFillColor(sf::Color::Cyan);
    bound.scale(0.03, 0.03);
    movementSpeed = 3;

    distance = 50;


}

void weapon::update() {
    if (direction==1){//up
        bound.move(0,-movementSpeed);
    }
    if (direction==2){//down
        bound.move(0,movementSpeed);
    }
    if (direction==3){//left
        bound.move(-movementSpeed,0);
    }
    if (direction==4){//right
        bound.move(movementSpeed,0);
    }
}