//
// Created by leonid on 26.04.19.
//

#include "GameCore.h"
#include "player.h"
#include "grenade.h"
#include "handWeapon.h"

void GameCore::addStaticObject(Level &lvl, b2World &world) {
    std::vector<Object> block = lvl.GetObjects("Platform");
    lvl.objInit(world, block);
}

void GameCore::createEntity(sf::RenderWindow &window, b2World &world, std::string object, int x, int y,
                            std::string playerTexture) {//Возможны проблемы
    if (object == "player") {
        class player Player1(world, Textures[playerTexture], x, y);
        Player1.magicnumber = 47+(int)players.size();
        players.push_back(Player1);


        //printf("Stored: %p, Real %p\n",players[players.size()-1].realBody->GetUserData(),&players[players.size()-1]);
    }
    if (object == "grenade") {
        class grenade grenade(world, window, Textures["texture_grenade"], Textures["texture_explosion"]);
        grenades.push_back(grenade);
    }
    if (object == "weapon") {
        class weapon weapon(world, window, Textures["textureWeapon"], Textures["textureBullet"],
                            Textures["texture_explosion"]);
        weapons.push_back(weapon);
    }
    if (object == "handWeapon") {
        class handWeapon bladeFire(world, Textures["textureBlade"], Textures["textureFire"]);
        handWeapons.push_back(bladeFire);
    }


}

void GameCore::createMovableObjects(sf::RenderWindow &window, b2World &world) {
    createEntity(window, world, "player", 400, 350, "textureSans");
    createEntity(window, world, "player", 200, 350, "textureSans");
    for (int j = 0; j < players.size(); ++j) {
        players[j].realBody->SetUserData(&players[j]);
    }

    for (int i = 0; i < 3; ++i) {
        createEntity(window, world, "grenade", 1, 1, "");
    }
    for (int j = 0; j < grenades.size(); ++j) {
        grenades[j].realBody->SetUserData(&grenades[j]);
    }
    createEntity(window, world, "weapon", 1, 1, "");
    createEntity(window, world, "handWeapon", 1, 1, "");

    for (int j = 0; j < weapons.size(); ++j) {
        weapons[j].realBody->SetUserData(&weapons[j]);
    }
    for (int j = 0; j < handWeapons.size(); ++j) {
        handWeapons[j].realBody->SetUserData(&handWeapons[j]);
    }

}
void GameCore::initLvl(b2World &world){

    lvl.LoadFromFile("/home/leonid/CLionProjects/Game/Map/TestEdited.tmx");

    std::vector<Object> block = lvl.GetObjects("Platform");
    lvl.objInit(world,block);
}
void GameCore::updateMap(sf::RenderWindow &window) {
    lvl.Draw(window);
    for (int i = 0; i < players.size(); ++i) {
        players[i].update();
        window.draw(players[i].sprite);
    }


    for (auto iter = grenades.begin(); iter!=grenades.end(); iter++) {
        if(iter->realBody!= nullptr) {
            window.draw(iter->sprite);
            iter->grenade_update();
        }
    }

    for (int j = 0; j < weapons.size(); ++j) {
        weapons[j].weapon_update();
        window.draw(weapons[j].sprite);
    }
    for (int j = 0; j < handWeapons.size(); ++j) {
        handWeapons[j].update(window);
        window.draw(handWeapons[j].sprite);
    }


}

void GameCore::loadTextures() {
    std::string Path_to_res = "/home/leonid/CLionProjects/Resourses/";
    std::string Path_to_duck = "/home/leonid/CLionProjects/Resourses/Duck Game Sprites/";

    sf::Texture textureWeapon;
    if (!textureWeapon.loadFromFile(Path_to_duck + "ak47.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    Textures["textureWeapon"] = textureWeapon;
    sf::Texture textureBullet;
    if (!textureBullet.loadFromFile(Path_to_duck + "chainBullet.png")) {
        printf("Ошибка загрузики\n");
        exit(1);

    }
    Textures["textureBullet"] = textureBullet;

    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }

    Textures["textureSans"] = textureSans;

    sf::Texture textureBlade;
    if (!textureBlade.loadFromFile(Path_to_res + "blade1.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }

    Textures["textureBlade"] = textureBlade;
    sf::Texture textureFire;
    if (!textureFire.loadFromFile(Path_to_res + "fireTexture.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    Textures["textureFire"] = textureFire;
    sf::Texture texture_explosion;
    if (!texture_explosion.loadFromFile(Path_to_duck + "airFire.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    Textures["texture_explosion"] = texture_explosion;
    sf::Texture texture_grenade;
    if (!texture_grenade.loadFromFile(Path_to_duck + "grenade.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    Textures["texture_grenade"] = texture_grenade;


}