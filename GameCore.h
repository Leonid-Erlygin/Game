//
// Created by leonid on 26.04.19.
//

#ifndef GAME_GAMECORE_H
#define GAME_GAMECORE_H


#include "object.h"
#include "Map/level.h"
#include "player.h"
#include "grenade.h"
#include "handWeapon.h"
#include <stdio.h>
#pragma once
class GameCore {
public:
    Level lvl;
    std::vector<player>players;
    std::vector<grenade>grenades;
    std::vector<weapon>weapons;
    std::vector<handWeapon>handWeapons;


    std::map<std::string,sf::Texture>Textures;
    void loadTextures();
    void initLvl(b2World &world);
    void addStaticObject(Level &,b2World&);
    void createEntity(sf::RenderWindow&,b2World &world,std::string,int x,int y,std::string playerTexture);

    void createMovableObjects(sf::RenderWindow&window,b2World &world);
    void updateMap(sf::RenderWindow &window);
};


#endif //GAME_GAMECORE_H
