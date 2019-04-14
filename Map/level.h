//
// Created by leonid on 13.04.19.
//

#ifndef LEVEL_H
#define LEVEL_H
#endif
/*
#pragma comment(lib,"Box2D.lib")
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-system.lib")
*/
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"

#pragma once

struct Object
{
    int GetPropertyInt(std::string name);
    float GetPropertyFloat(std::string name);
    std::string GetPropertyString(std::string name);

    std::string name;
    std::string type;
    sf::Rect<int> rect;
    std::map<std::string, std::string> properties;

    sf::Sprite sprite;
};

struct Layer
{
    int opacity;
    std::vector<sf::Sprite> tiles;
};

class Level
{
public:
    void objInit(b2World&world, std::vector<Object>&block);
    bool LoadFromFile(std::string filename);
    Object GetObject(std::string name);
    std::vector<Object> GetObjects(std::string name);
    void Draw(sf::RenderWindow &window);
    sf::Vector2i GetTileSize();
    float32 scale_factorX = 40;//1 м = 40 pxl
    float32 scale_factorY = -40;

private:
    int width, height, tileWidth, tileHeight;
    int firstTileID;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;
};
