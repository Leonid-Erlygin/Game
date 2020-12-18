//
// Created by leonid on 12.02.19.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#pragma once

#include "object.h"
#include "weapon.h"

#include <SFML/Network.hpp>
#include <set>

class player : public object {
public:

    int counter = 0;

    void print();
    sf::IpAddress serverIp;
    int serverPort;
    bool grab = false;
    float speed = 10;
    float strength = 3;
    std::set<object *> reachableObjects;
    object *cariedObject;
    int remainingJumpSteps = 1;
    int jumpHeight = 200;
    b2RevoluteJoint *JointToHold = nullptr;
    bool canTake = false;
    bool moveRight = false;
    bool moveLeft = false;
    // int direction = 0;// -1 = left, 1 = right

    bool is_on_ground();

    int is_on_wall();

    b2World &world;

    void grabe(b2World &, object *flipObject = nullptr, bool is_flip = false);

    void update(std::vector<sf::UdpSocket> &, int player_index, bool need_to_send);

    void death(int x, int y);


    void checkEvents(std::vector<sf::UdpSocket> &, sf::Event &, b2World &, int player, int player_index);

    player(b2World &world, sf::Texture &, sf::SoundBuffer &jump_buffer, int x, int y);

    void throwObject(b2Body &);

private:

    sf::Sound jump_sound;

    int scale_factorX = 40;
    int scale_factorY = -40;
};


#endif //GAME_PLAYER_H
