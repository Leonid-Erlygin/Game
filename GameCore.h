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
#include "virtualPlayer.h"
#include <stdio.h>
#pragma once
class GameCore {
public:
    Level lvl;
    std::vector<player>players;
    std::vector<grenade>grenades;
    std::vector<weapon>weapons;
    std::vector<handWeapon>handWeapons;
    std::vector<virtualPlayer>virtualPlayers;
    GameCore(sf::RenderWindow&,b2World&,int x);
    b2World& world;
    int x;
    sf::RenderWindow& window;
    std::map<std::string,sf::Texture>Textures;
    std::map<std::string,sf::SoundBuffer>SoundBuffers;

    void loadTextures();
    void runLevel(std::vector<sf::UdpSocket>&);
    void runMenu();
    void initLvl();
    void addStaticObject(Level &,b2World&);
    void createEntity(std::string,int x,int y,std::string playerTexture);
    void loadSoundBuffers();
    void createMovableObjects();
    void updateMap(std::vector<sf::UdpSocket> &socket,int x);

private:
    void mapBuilder(std::string);
    inline int  runLoop(int x11,int y11,int x12,int y12,int x21,int y21,int x22,int y22, int scaleX, int scaleY
    ,int posX, int posY, std::string name);
    inline void runIp(int scaleX, int scaleY,int posX,int posY,std::string& nameString, std::string& ipString);
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
};


#endif //GAME_GAMECORE_H
