//
// Created by leonid on 26.04.19.
//

#include <iostream>
#include <fstream>
#include "GameCore.h"
#include "player.h"
#include "grenade.h"
#include "handWeapon.h"


GameCore::GameCore(sf::RenderWindow &window1, b2World &world1, int x) : window(window1), world(world1),x(x) {

}
void GameCore::loadSoundBuffers()
{
    std::string Path_to_res = "/home/leonid/CLionProjects/Game/SFX/";
    sf::SoundBuffer shot_buffer;
    if (!shot_buffer.loadFromFile(Path_to_res + "shotgun.wav"))
    {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["ak47"] = shot_buffer;

    sf::SoundBuffer fire_buffer;
    if (!fire_buffer.loadFromFile(Path_to_res + "flameThrowing.wav"))
    {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["fire"] = fire_buffer;
}


void GameCore::addStaticObject(Level &lvl, b2World &world) {
    std::vector<Object> block = lvl.GetObjects("Platform");
    lvl.objInit(world, block);
}


inline std::pair<float, float> getMedian(std::vector<player> &players) {
    if (players.size() != 1) {
        sf::Vector2f m;
        for (int i = 0; i < players.size(); ++i) {
            m += players[i].bound.getPosition();
        }

        std::pair<float, float> temp(m.x / 2, m.y / 2);
        return temp;
    } else {
        sf::Vector2f m = players[0].bound.getPosition();
        std::pair<float, float> temp(m.x, m.y);
        return temp;
    }
}

//in process...
inline sf::Vector2f zoom(player const &PlayerA, player const &PlayerB, sf::RenderWindow &window) {


    sf::Vector2f c = PlayerB.bound.getPosition() - PlayerA.bound.getPosition();//proportions
    float sizeWin = window.getSize().x * window.getSize().x + window.getSize().y * window.getSize().y;
    if (c.x < 0)c.x = -c.x;
    float k = (float) window.getSize().y / (float) window.getSize().x;
    if (c.y < 0)c.y = -c.y;


    float d = c.x * c.x + c.y * c.y;//think about function
    int frac = 3;
    float capacity = 2.0f;
    if (sqrt(d) < sqrt(sizeWin) / frac) {
        return sf::Vector2f(capacity * sqrt(sizeWin / (1 + k * k)) / frac,
                            capacity * (sqrt(sizeWin / (1 + k * k)) / frac) * k);
    } else {


        return sf::Vector2f(capacity * (sqrt(d / (1 + k * k))), capacity * (sqrt(d / (1 + k * k))) * k);

    }
}

void GameCore::runLevel(std::vector<sf::UdpSocket> &socket) {


    sf::View view1;
    view1.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    view1.setCenter(sf::Vector2f(view1.getSize().x / 2, view1.getSize().y / 2));
    window.setView(view1);
    std::pair<float, float> median;

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed)
                window.close();
            for (int i = 0; i < players.size(); ++i) {
                players[i].checkEvents(socket, event, world, i + 1, x);
            }

        }

        window.clear(sf::Color::Black);

        world.Step(timeStep, velocityIterations, positionIterations);


        updateMap(socket, x);

        window.setView(view1);
        median = getMedian(players);
        view1.setCenter(sf::Vector2f(median.first, median.second));
        if (players.size() > 1) {
            view1.setSize(zoom(players[0], players[1], window));
        }

        window.display();

    }
}


inline int
GameCore::runLoop(int x11, int x12, int y11, int y12, int x21, int x22, int y21, int y22, int scaleX, int scaleY,
                  int posX, int posY, std::string name) {
    std::string Path_to_res = "/home/leonid/CLionProjects/Resourses/";
    sf::Texture textureMenu2;
    if (!textureMenu2.loadFromFile(Path_to_res + name)) {
        printf("Ошибка загрузики\n");
        exit(1);
    }


    sf::Sprite menu2;
    menu2.setTexture(textureMenu2);
    menu2.setScale(scaleX, scaleY);
    menu2.setPosition(sf::Vector2f(posX, posY));
    bool run = true;
    int h = 0;
    while (window.isOpen() && run) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.y > (y11 * scaleY + posY) && event.mouseButton.y<(y12 * scaleY + posY)
                                                                                           && event.mouseButton.x>(
                            x11 * scaleX + posX) && event.mouseButton.x < (x12 * scaleX + posX)) {
                        run = false;
                        h = 1;
                    }
                    if (event.mouseButton.y > (y21 * scaleY + posY) && event.mouseButton.y<(y22 * scaleY + posY)
                                                                                           && event.mouseButton.x>(
                            x21 * scaleX + posX) && event.mouseButton.x < (x22 * scaleX + posX)) {
                        run = false;
                        h = 2;
                    }
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(menu2);
        window.display();

    }
    return h;
}

inline void GameCore::runIp(int scaleX, int scaleY,int posX,int posY,std::string& nameString, std::string& ipString){
    std::string Path_to_res = "/home/leonid/CLionProjects/Resourses/";
    sf::Font font;
    if (!font.loadFromFile(Path_to_res + "arial.ttf")) {
        exit(0);
    }
    sf::Texture textureIP;
    if (!textureIP.loadFromFile(Path_to_res + "ip.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    sf::Sprite ip;
    ip.setTexture(textureIP);
    ip.setScale(scaleX, scaleY);
    ip.setPosition(sf::Vector2f(posX, posY));
    sf::Text textIp;
    textIp.setFont(font);
    int size = 37;
    textIp.setCharacterSize(size);
    sf::Text textName;
    textName.setFont(font);
    textName.setCharacterSize(size);

    textIp.setPosition(posX + 19 * scaleX, posY + 20 * scaleY);
    textName.setPosition(posX + 19 * scaleX, posY + 36 * scaleY);

    textName.setString(nameString);
    textIp.setString(ipString);

    bool run = true;
    int ind = 0;
    int div = 20;
    int lim = 20;
    int writingCount = lim;
    bool waiting = true;
    bool ipField = false;
    bool nameField = false;
    if(!nameString.empty()){
        nameField = true;
    }
    if(!ipString.empty()){
        nameField = false;
        ipField = true;
    }
    while (window.isOpen() && run) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left) {

                    if (event.mouseButton.x > (18 * scaleX + posX) && event.mouseButton.x<(71 * scaleX + posX)
                                                                                          &&
                                                                                          event.mouseButton.y>(
                            17 * scaleY + posY) && event.mouseButton.y < (30 * scaleY + posY)) {

                        ipField = true;
                        nameField = false;
                        if (nameString[nameString.size() - 1] == '|') {
                            nameString = nameString.substr(0, nameString.size() - 1);
                            textName.setString(nameString);
                        }
                    }
                    if (event.mouseButton.x > (18 * scaleX + posX) && event.mouseButton.x<(71 * scaleX + posX)
                                                                                          &&
                                                                                          event.mouseButton.y>(
                            33 * scaleY + posY) && event.mouseButton.y < (47 * scaleY + posY)) {
                        nameField = true;
                        ipField = false;
                        if (ipString[ipString.size() - 1] == '|') {
                            ipString = ipString.substr(0, ipString.size() - 1);
                            textIp.setString(ipString);
                        }

                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Return){
                    run = false;
                }
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (ipField) {
                        ipString = ipString.substr(0, ipString.size() - 1);
                        textIp.setString(ipString);
                    }
                    if (nameField) {
                        nameString = nameString.substr(0, nameString.size() - 1);
                        textName.setString(nameString);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (nameField) {
                    writingCount = 0;
                    waiting = false;
                    if (event.text.unicode < 128) {
                        char a = static_cast<char>(event.text.unicode);
                        if (a >=32) {
                            if (nameString[nameString.size() - 1] == '|')
                                nameString = nameString.substr(0,
                                                               nameString.size() -
                                                               1);
                            nameString += a;
                            textName.setString(nameString);
                        }
                    }
                }
                if (ipField) {
                    writingCount = 0;
                    waiting = false;
                    if (event.text.unicode < 128) {
                        char a = static_cast<char>(event.text.unicode);
                        if (a >=32) {
                            if (ipString[ipString.size() - 1] == '|')
                                ipString = ipString.substr(0,
                                                           ipString.size() -
                                                           1);
                            ipString += a;
                            textIp.setString(ipString);
                        }

                    }
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(ip);
        if (ipField && waiting) {
            if (((ind / div) % 2 == 0 && ipString[ipString.size() - 1] != '|'))
                ipString = ipString + "|";
            if ((ind / div) % 2 == 1 && ipString[ipString.size() - 1] == '|')
                ipString = ipString.substr(0, ipString.size() - 1);

            textIp.setString(ipString);

        }
        if (nameField && waiting) {
            if ((ind / div) % 2 == 0 && nameString[nameString.size() - 1] != '|')
                nameString += "|";
            if ((ind / div) % 2 == 1 && nameString[nameString.size() - 1] == '|')
                nameString = nameString.substr(0, nameString.size() - 1);

            textName.setString(nameString);
        }
        ind++;
        ind = ind % 1000;
        writingCount++;
        if (writingCount > lim)waiting = true;
        writingCount %= 50;
        window.draw(textIp);
        window.draw(textName);

        window.display();
    }
}

void GameCore::runMenu() {
    std::string Path_to_res = "/home/leonid/CLionProjects/Resourses/";
    sf::Texture textureMenu1;
    int scaleX = 24;
    int scaleY = scaleX;
    if (!textureMenu1.loadFromFile(Path_to_res + "mainMenu.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    int posX = 200;
    int posY = posX;
    std::vector<sf::Sprite> Menu1;
    for (int i = 0; i < 26; ++i) {
        sf::Sprite sp;
        sp.setTexture(textureMenu1);
        int k = textureMenu1.getSize().x / 27;
        sp.setScale(scaleX, scaleY);
        sp.setPosition(sf::Vector2f(posX, posY));
        sp.setTextureRect(sf::IntRect(i * k, 0, k, textureMenu1.getSize().y));
        Menu1.push_back(sp);
    }
    int i = 0;
    bool run = true;
    while (window.isOpen() && run) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.y > (4 * scaleY + posY) &&
                        event.mouseButton.y<(12 * scaleY + posY) && event.mouseButton.x>(6 * scaleX + posX) &&
                        event.mouseButton.x < (23 * scaleX + posX))
                        run = false;
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(Menu1[i / 3]);
        i++;
        i = i % 76;
        window.display();
    }
    scaleX = scaleX / 3;
    scaleY = scaleY / 3;
    int h = runLoop(12, 37, 16, 28, 52, 77, 16, 28, scaleX, scaleY, posX, posY, "Menu2.png");


    if (h == 1) {//local
        return;
    } else {
        int g = runLoop(5, 40, 14, 30, 49, 85, 14, 30, scaleX, scaleY, posX, posY, "menu3.png");

        if (g == 1) {//join
            std::string nameString;
            std::string ipString;
            while (nameString.empty())
            runIp(scaleX,scaleY,posX,posY,nameString,ipString);


        } else {//create

        }
    }

}


void GameCore::createEntity(std::string object, int x, int y,
                            std::string playerTexture) {//Возможны проблемы

    if (object == "player") {

        class player Player1(world, Textures[playerTexture], x, y);
        Player1.magicnumber = 47 + (int) players.size();
        players.push_back(Player1);

    }
    if (object == "virtualPlayer") {

        class virtualPlayer Player1(world, Textures[playerTexture], x, y);
        Player1.magicnumber = 47 + (int) players.size();
        virtualPlayers.push_back(Player1);

    }
    if (object == "grenade") {
        class grenade grenade(world, window, Textures["texture_grenade"], Textures["texture_explosion"],SoundBuffers["ak47"],x,y);
        grenades.push_back(grenade);
    }
    if (object == "weapon") {
        class weapon weapon(world, window, Textures["textureWeapon"], Textures["textureBullet"],
                            Textures["texture_explosion"],SoundBuffers["ak47"],x,y);
        weapons.push_back(weapon);
    }
    if (object == "handWeapon") {
        class handWeapon bladeFire(world, Textures["textureBlade"], Textures["textureFire"],SoundBuffers["fire"],x,y);
        handWeapons.push_back(bladeFire);
    }


}

void GameCore::createMovableObjects() {

    if (x == 1) {
        createEntity("player", 400, 350, "textureSans");
        createEntity("virtualPlayer", 200, 350, "textureSans");
    } else {
        createEntity("virtualPlayer", 400, 350, "textureSans");
        createEntity("player", 200, 350, "textureSans");
    }

    //createEntity(window, world, "player", 200, 350, "textureSans");
    for (int j = 0; j < players.size(); ++j) {
        players[j].realBody->SetUserData(&players[j]);
    }


    for (int j = 0; j < virtualPlayers.size(); ++j) {
        virtualPlayers[j].realBody->SetUserData(&virtualPlayers[j]);
    }

    for (int i = 0; i < 6; ++i) {
        createEntity("grenade", 250, 360, "");
    }
    for (int j = 0; j < grenades.size(); ++j) {
        grenades[j].realBody->SetUserData(&grenades[j]);
    }
    createEntity("weapon", 200, 360, "");
    createEntity("handWeapon", 600, 360, "");

    for (int j = 0; j < weapons.size(); ++j) {
        weapons[j].realBody->SetUserData(&weapons[j]);
    }
    for (int j = 0; j < handWeapons.size(); ++j) {
        handWeapons[j].realBody->SetUserData(&handWeapons[j]);
    }

}

void GameCore::initLvl() {

    lvl.LoadFromFile("/home/leonid/CLionProjects/Game/Map/TestEdited.tmx");

    std::vector<Object> block = lvl.GetObjects("Platform");
    lvl.objInit(world, block);
}

void GameCore::updateMap(std::vector<sf::UdpSocket> &socket, int x) {
    lvl.Draw(window);
    for (int i = 0; i < players.size(); ++i) {
        players[i].update();
        window.draw(players[i].sprite);
    }
    for (int k = 0; k < virtualPlayers.size(); ++k) {
        virtualPlayers[k].update(socket, world, x);
        window.draw(virtualPlayers[k].sprite);
    }

    for (auto iter = grenades.begin(); iter != grenades.end(); iter++) {
        if (iter->realBody != nullptr) {
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

void GameCore::mapBuilder(std::string name){

    std::ifstream inFile;
    std::ofstream outFile;
    std::string s;
    outFile.open("/home/leonid/CLionProjects/Resourses/Map/"+name+"Edited");
    inFile.open("/home/leonid/CLionProjects/Resourses/Map/"+name);
    while (getline(inFile,s)){
        if (s.find("<tile")!=-1){
            if(s.find("gid")==-1){
                s =  "<tile gid=\"0\"/>";
            }
        }
        s +="\n";
        outFile<<s;

    }
    inFile.close();
    outFile.close();

}