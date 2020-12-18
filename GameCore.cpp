//
// Created by leonid on 26.04.19.
//

#include <iostream>
#include <fstream>
#include "GameCore.h"
#include "player.h"
#include "grenade.h"
#include "handWeapon.h"
#include "Map/level.h"

GameCore::GameCore(sf::RenderWindow &window1, b2World &world1,
                   int player_index, int send_period, int number_of_pl) :
        window(window1),
        world(world1),
        player_index(player_index),
        send_period(send_period),
        number_of_players(number_of_pl){

}

void GameCore::loadSoundBuffers() {
    std::string Path_to_res = "../Resourses/Sounds/";

    sf::SoundBuffer shot_buffer;
    if (!shot_buffer.loadFromFile(Path_to_res + "shotgun.wav")) {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["ak47"] = shot_buffer;

    sf::SoundBuffer fire_buffer;
    if (!fire_buffer.loadFromFile(Path_to_res + "flameThrowing.wav")) {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["fire"] = fire_buffer;

    sf::SoundBuffer magnum_buffer;
    if (!magnum_buffer.loadFromFile(Path_to_res + "magnum.wav")) {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["magnum"] = magnum_buffer;

    sf::SoundBuffer sniper_buffer;
    if (!sniper_buffer.loadFromFile(Path_to_res + "sniper.wav")) {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["sniper"] = sniper_buffer;

    sf::SoundBuffer jump_buffer;
    if (!jump_buffer.loadFromFile(Path_to_res + "jump.wav")) {
        printf("failure when loading sound buffer\n");
        exit(1);
    }
    SoundBuffers["jump"] = jump_buffer;
}


void GameCore::addStaticObject(Level &lvl, b2World &world) {
    std::vector<Object> block = lvl.GetObjects("Platform");
    lvl.objInit(world, block);
}


inline std::pair<float, float> getMedian(std::vector<player> &players, std::vector<virtualPlayer> &virt) {
    if (players.size() != 1) {
        sf::Vector2f m;
        for (size_t i = 0; i < players.size(); ++i) {
            m += players[i].bound.getPosition();
        }

        std::pair<float, float> temp(m.x / 2, m.y / 2);
        return temp;
    } else {
        if (virt.size() == 1) {
            sf::Vector2f m;
            m += players[0].bound.getPosition();
            m += virt[0].bound.getPosition();
            std::pair<float, float> temp(m.x / 2, m.y / 2);
            return temp;
        } else {
            sf::Vector2f m = players[0].bound.getPosition();
            std::pair<float, float> temp(m.x, m.y);
            return temp;
        }
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

    if (isLocal) {
        std::pair<b2Vec2, b2Vec2> pos;
        std::vector<Object> player1 = lvl.GetObjects("player1");
        std::vector<Object> player2 = lvl.GetObjects("player2");
        float m = lvl.mapScaleX * (player1[0].rect.left + player1[0].rect.width / 2.f);
        float n = lvl.mapScaleY * (player1[0].rect.top + player1[0].rect.height / 2.f);
        pos.first.x = m;
        pos.first.y = n;
        pos.second.x = lvl.mapScaleX * (player2[0].rect.left + player2[0].rect.width / 2.f);
        pos.second.y = lvl.mapScaleY * (player2[0].rect.top + player2[0].rect.height / 2.f);

        createMovableObjects(pos);

        std::string Path_to_res = "../Resourses/";

        if (!font.loadFromFile(Path_to_res + "arial.ttf")) {
            exit(0);
        }

        for (int i = 0; i < playerName.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(playerName[i]);
            text.setCharacterSize(15);
            names.push_back(text);
        }

    } else {
        std::pair<b2Vec2, b2Vec2> pos;
        std::vector<Object> player1 = lvl.GetObjects("player1");
        std::vector<Object> player2 = lvl.GetObjects("player2");
        float m = lvl.mapScaleX * (player1[0].rect.left + player1[0].rect.width / 2.f);
        float n = lvl.mapScaleY * (player1[0].rect.top + player1[0].rect.height / 2.f);
        pos.first.x = m;
        pos.first.y = n;
        pos.second.x = lvl.mapScaleX * (player2[0].rect.left + player2[0].rect.width / 2.f);
        pos.second.y = lvl.mapScaleY * (player2[0].rect.top + player2[0].rect.height / 2.f);

        createMovableObjects(pos);

        std::string Path_to_res = "../Resourses/";

        if (!font.loadFromFile(Path_to_res + "arial.ttf")) {
            exit(0);
        }

        for (int i = 0; i < playerName.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(playerName[i]);
            text.setCharacterSize(15);
            names.push_back(text);
        }
        for (int i = 0; i <players.size(); ++i) {
            players[i].serverIp = serverIp;
            players[i].serverPort = serverPort;
        }
    }

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed)
                window.close();
            for (size_t i = 0; i < players.size(); ++i) {
                players[i].checkEvents(socket, event, world, i + 1, player_index);
            }

        }

        window.clear(sf::Color::Black);

        world.Step(timeStep, velocityIterations, positionIterations);


        updateMap(socket);

        window.setView(view1);
        median = getMedian(players, virtualPlayers);
        view1.setCenter(sf::Vector2f(median.first, median.second));
        if (players.size() > 1) {
            view1.setSize(zoom(players[0], players[1], window));
        }
        if (virtualPlayers.size() == 1) {
            view1.setSize(zoom(players[0], virtualPlayers[0], window));
        }

        window.display();

    }
}


inline int
GameCore::runLoop(int x11, int x12, int y11, int y12, int x21, int x22, int y21, int y22, float scaleX, float scaleY,
                  int posX, int posY, std::string name) {
    std::string Path_to_res = "../Resourses/";
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

inline void
GameCore::runIp(int x11, int x12, int y11, int y12, int x21, int x22, int y21, int y22, float scaleX, float scaleY,
                int posX, int posY, std::string &nameString, std::string &ipString, std::string name) {
    /*
     * Allows to enter the text in a box
     *
     */
    std::string Path_to_res = "../Resourses/";
    sf::Font font;
    if (!font.loadFromFile(Path_to_res + "arial.ttf")) {
        exit(0);
    }
    sf::Texture textureIP;
    if (!textureIP.loadFromFile(Path_to_res + name)) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    sf::Sprite ip;
    ip.setTexture(textureIP);
    ip.setScale(scaleX, scaleY);
    ip.setPosition(sf::Vector2f(posX, posY));
    sf::Text textIp;
    textIp.setFont(font);
    int size = 40;
    textIp.setCharacterSize(size);
    sf::Text textName;
    textName.setFont(font);
    textName.setCharacterSize(size);

    textIp.setPosition(posX + x11 * scaleX, posY + y11 * scaleY);
    textName.setPosition(posX + x21 * scaleX, posY + y21 * scaleY);

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
    if (!nameString.empty()) {
        nameField = true;
    }
    if (!ipString.empty()) {
        nameField = false;
        ipField = true;
    }
    while (window.isOpen() && run) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left) {

                    if (event.mouseButton.x > (x11 * scaleX + posX) && event.mouseButton.x<(x12 * scaleX + posX)
                                                                                           &&
                                                                                           event.mouseButton.y>(
                            y11 * scaleY + posY) && event.mouseButton.y < (y12 * scaleY + posY)) {

                        ipField = true;
                        nameField = false;
                        if (nameString[nameString.size() - 1] == '|') {
                            nameString = nameString.substr(0, nameString.size() - 1);
                            textName.setString(nameString);
                        }
                    }
                    if (event.mouseButton.x > (x21 * scaleX + posX) && event.mouseButton.x<(x22 * scaleX + posX)
                                                                                           &&
                                                                                           event.mouseButton.y>(
                            y21 * scaleY + posY) && event.mouseButton.y < (y22 * scaleY + posY)) {
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
                if (event.key.code == sf::Keyboard::Return) {
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
                        if (a >= 32) {
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
                        if (a >= 32) {
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

    std::string Path_to_res = "../Resourses/";
    sf::Texture textureMenu1;
    float scaleX = 60;
    float scaleY = scaleX;
    if (!textureMenu1.loadFromFile(Path_to_res + "mainMenu.png")) {
        printf("Ошибка загрузики\n");
        exit(1);
    }
    int posX = 0;
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
    bool run = true; //here we decide not to display menu
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
    scaleX = 1.414;
    scaleY = scaleX;

    int h = runLoop(486, 887, 418, 458, 486, 887, 492, 532, scaleX, scaleY, posX, posY, "Menu2.png");
    //int h = 2;
    std::string nameString;
    std::string ipString;
    if (h == 1) {//local
        isLocal = true;
        // HERE IP STRING IS ACTUALLY THE NAME OF SECOND PLAYER
        while (nameString.empty() || ipString.empty())
            runIp(223, 651, 401, 444, 702, 1140, 401, 446, scaleX, scaleY, posX, posY, nameString, ipString,
                  "localNames.png");

        if (nameString[nameString.size() - 1] == '|')nameString = nameString.substr(0, nameString.size() - 1);
        if (ipString[ipString.size() - 1] == '|')ipString = ipString.substr(0, ipString.size() - 1);

        playerName.push_back(nameString);
        playerName.push_back(ipString);
        return;
    } else {
        while (nameString.empty()||ipString.empty())
            runIp(488, 888, 421, 460, 490, 891, 492, 533, scaleX, scaleY, posX, posY, nameString, ipString, "Ip.png");

        if (nameString[nameString.size() - 1] == '|')nameString = nameString.substr(0, nameString.size() - 1);
        if (ipString[ipString.size() - 1] == '|')ipString = ipString.substr(0, ipString.size() - 1);
        playerName.push_back(nameString);
        std::string s1 = ipString.substr(0, ipString.find(":"));

        std::string s2 = ipString.substr(ipString.find(':') + 1,ipString.size()-1);

        serverIp = s1;
        serverPort = std::atoi(s2.c_str());
//          serverIp = "10.55.160.246";
//          //serverIp = sf::IpAddress::LocalHost;
//          serverPort = 54002;
    }


}


void GameCore::createEntity(std::string object, int x, int y,
                            std::string playerTexture, int whose = 0) {//Возможны проблемы

    if (object == "player") {

        class player Player1(world, Textures[playerTexture], SoundBuffers["jump"], x, y);
        Player1.magicnumber = 47 + (int) players.size();
        players.push_back(Player1);

    }
    if (object == "virtualPlayer") {

        class virtualPlayer Player1(world, Textures[playerTexture], SoundBuffers["jump"], x, y);
        Player1.magicnumber = 47 + (int) players.size();
        //Player1.whose_avatar = whose;
        virtualPlayers.push_back(Player1);


    }
    if (object == "grenade") {
        class grenade grenade(world, window, Textures["grenade"], Textures["explosion"], SoundBuffers["sniper"], x, y);
        grenades.push_back(grenade);
    }
    if (object == "weapon") {
        class weapon ak47(world, window, Textures["ak47"], Textures["Bullet"],
                          Textures["shot"], SoundBuffers["ak47"], x, y);
        weapons.push_back(ak47);
    }
    if (object == "weapon1") {
        class weapon sniper(world, window, Textures["sniper"], Textures["Bullet"],
                            Textures["shot"], SoundBuffers["sniper"], x, y, 0.7, 30, 40 * (3.14159265 / 180), 20);
        weapons.push_back(sniper);
    }
    if (object == "weapon2") {
        class weapon magnum(world, window, Textures["magnum"], Textures["Bullet"],
                            Textures["shot"], SoundBuffers["magnum"], x, y, 0.22, 20, 5 * (3.14159265 / 180), 3);
        weapons.push_back(magnum);
    }
    if (object == "handWeapon") {
        class handWeapon bladeFire(world, Textures["Blade"], Textures["Fire"], SoundBuffers["fire"], x, y);
        handWeapons.push_back(bladeFire);
    }


}

void GameCore::createMovableObjects(std::pair<b2Vec2, b2Vec2> playerPos) {
    if (isLocal) {
        createEntity("player", playerPos.first.x, playerPos.first.y, "textureSans");
        createEntity("player", playerPos.second.x, playerPos.second.y, "textureSans");


    } else {
        for (int i = 1; i <=number_of_players; ++i) {

            if (i == player_index){
                createEntity("player", 100 * i, 0, "textureSans");
                continue;
            }
            createEntity("virtualPlayer", 100 * i, 0, "textureSans", i);

        }

    }


    for (size_t j = 0; j < players.size(); ++j) {
        players[j].realBody->SetUserData(&players[j]);
    }


    for (size_t j = 0; j < virtualPlayers.size(); ++j) {
        virtualPlayers[j].realBody->SetUserData(&virtualPlayers[j]);
    }

    for (int i = 0; i < 2; ++i) {
        createEntity("grenade", 700 + i * 100, 360, "");
    }
    for (size_t j = 0; j < grenades.size(); ++j) {
        grenades[j].realBody->SetUserData(&grenades[j]);
    }
    createEntity("weapon1", 200 + 200, 500, "");
    createEntity("weapon2", 200 + 400, 500, "");
    createEntity("weapon", 1000, 360, "");
    createEntity("handWeapon", 600, 360, "");

    for (size_t j = 0; j < weapons.size(); ++j) {
        weapons[j].realBody->SetUserData(&weapons[j]);
    }
    for (size_t j = 0; j < handWeapons.size(); ++j) {
        handWeapons[j].realBody->SetUserData(&handWeapons[j]);
    }

}

void GameCore::initLvl(std::string name) {

    //mapBuilder(name);

    lvl.LoadFromFile("../Map/" + name + "/level2.tmx");

    std::vector<Object> block = lvl.GetObjects("block");
    lvl.objInit(world, block);
}

void GameCore::updateMap(std::vector<sf::UdpSocket> &socket) {
    // this function is called every frame
    lvl.Draw(window);
    //update players
    for (size_t i = 0; i < players.size(); ++i) {
        if (steps_past % send_period == 0) {
            players[i].update(socket, player_index, true);
            steps_past = 0;
        } else {
            players[i].update(socket, player_index, false);
        }
        if (!names.empty()){
            names[i].setPosition(players[i].bound.getPosition().x - players[i].bound.getSize().x / 12,
                                 players[i].bound.getPosition().y - players[i].bound.getSize().y / 5);
            window.draw(names[i]);
        }

        window.draw(players[i].sprite);

    }
    steps_past++;

    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;
    int c = -500;
    int event = -300;
    int key_code = -1;
    int event_player_idx = -1;
    if (socket[0].receive(packet, sender, port) == sf::Socket::Done) {
        //printf("got_package!\n");
        packet >> c;
    };
    if (c == -500) { // nothing happened, just update game physics
        for (size_t k = 0; k < virtualPlayers.size(); ++k) {

            virtualPlayers[k].update(socket, world, true);
            window.draw(virtualPlayers[k].sprite);
        }
    } else if (c == -1) {//received coordinates of all virtual players
        float x = 0;
        float y = 0;
        float vx = 0;
        float vy = 0;
        for (size_t k = 0; k < virtualPlayers.size(); ++k) {
            packet >> x;
            packet >> y;
            packet >> vx;
            packet >> vy;
            virtualPlayers[k].update(socket, world, false, c, -2, -2, x, y, vx, vy);
            window.draw(virtualPlayers[k].sprite);
        }
    } else if (c == -2) {
        packet >> event_player_idx;
        packet >> event;
        packet >> key_code;
        float x = 0;
        float y = 0;
        float vx = 0;
        float vy = 0;
        packet >> x;
        packet >> y;
        packet >> vx;
        packet >> vy;
        int corresponding_place;
        if (event_player_idx < player_index) {
            //printf("1\n");
            corresponding_place = event_player_idx - 1;
        } else {
            if (event_player_idx == player_index) {
                exit(1);
            }
            corresponding_place = event_player_idx - 2;
        }

        for (size_t k = 0; k < virtualPlayers.size(); ++k) {
            if (k == corresponding_place) {
                virtualPlayers[k].update(socket, world, false, c, event, key_code, x, y, vx, vy);
            } else { // usual update
                virtualPlayers[k].update(socket, world, true);
            }

            window.draw(virtualPlayers[k].sprite);
        }
    }

    for (auto iter = grenades.begin(); iter != grenades.end(); iter++) {
        /*if (iter->realBody == nullptr && iter->explosion_sprites.size() == 0)
        {
        	continue;
        }*/
        iter->grenade_update();
    }

    for (size_t j = 0; j < weapons.size(); ++j) {
        weapons[j].weapon_update();

    }
    for (size_t j = 0; j < handWeapons.size(); ++j) {
        handWeapons[j].update(window);

    }


}

void GameCore::loadTextures() {
    std::string Path_to_res = "../Resourses/";
    std::string Path_to_duck = Path_to_res+"Duck Game Sprites/";

    sf::Texture texture_ak47;
    if (!texture_ak47.loadFromFile(Path_to_duck + "ak47.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["ak47"] = texture_ak47;

    sf::Texture texture_sniper;
    if (!texture_sniper.loadFromFile(Path_to_duck + "sniper.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["sniper"] = texture_sniper;

    sf::Texture texture_magnum;
    if (!texture_magnum.loadFromFile(Path_to_duck + "magnum.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["magnum"] = texture_magnum;

    sf::Texture textureBullet;
    if (!textureBullet.loadFromFile(Path_to_duck + "chainBullet.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);

    }
    Textures["Bullet"] = textureBullet;

    sf::Texture textureSans;
    if (!textureSans.loadFromFile(Path_to_res + "sans.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["textureSans"] = textureSans;

    sf::Texture textureBlade;
    if (!textureBlade.loadFromFile(Path_to_res + "blade1.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["Blade"] = textureBlade;

    sf::Texture textureFire;
    if (!textureFire.loadFromFile(Path_to_res + "fireTexture.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["Fire"] = textureFire;

    sf::Texture texture_shot;
    if (!texture_shot.loadFromFile(Path_to_duck + "airFire.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["shot"] = texture_shot;

    sf::Texture texture_explosion;
    if (!texture_explosion.loadFromFile(Path_to_duck + "explode.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["explosion"] = texture_explosion;

    sf::Texture texture_grenade;
    if (!texture_grenade.loadFromFile(Path_to_duck + "grenade.png")) {
        printf("РћС€РёР±РєР° Р·Р°РіСЂСѓР·РёРєРё\n");
        exit(1);
    }
    Textures["grenade"] = texture_grenade;


}

void GameCore::mapBuilder(std::string name) {

    std::ifstream inFile;
    std::ofstream outFile;
    std::string s;
    outFile.open("../Resourses/Map/" + name + "/TestEdited");
    inFile.open("../Resourses/Map/" + name + "level.tmx");
    while (getline(inFile, s)) {
        if (s.find("<tile") != -1) {
            if (s.find("gid") == -1) {
                s = "<tile gid=\"0\"/>";
            }
        }
        s += "\n";
        outFile << s;

    }
    inFile.close();
    outFile.close();

}
