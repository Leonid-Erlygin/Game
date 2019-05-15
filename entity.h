//
// Created by leonid on 11.02.19.
//

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class entity {
public:
    sf::RectangleShape bound;
    sf::Sprite sprite;
    sf::Text text;
};


#endif //GAME_ENTITY_H
