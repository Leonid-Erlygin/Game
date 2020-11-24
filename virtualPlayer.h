//
// Created by leonid on 03.03.19.
//

#ifndef GAME_VIRTUALPLAYER_H
#define GAME_VIRTUALPLAYER_H


#include "player.h"

class virtualPlayer : public player {
public:
    void  update(std::vector<sf::UdpSocket>&,b2World&, int player_index);
    virtualPlayer(b2World &world, sf::Texture &Player_texture,sf::SoundBuffer&,int x, int y);

private:
    int sinhrCount = 0;
    int sinhrLim = 0;
};


#endif //GAME_VIRTUALPLAYER_H
