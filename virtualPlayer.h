//
// Created by leonid on 03.03.19.
//

#ifndef GAME_VIRTUALPLAYER_H
#define GAME_VIRTUALPLAYER_H


#include "player.h"

class virtualPlayer : public player {
public:
    //int whose_avatar;
    void  update(std::vector<sf::UdpSocket>&,b2World&, int player_index, bool update_without_package, int = -500,int event = -2,int key = -3,float = 0 ,float = 0,float = 0 ,float = 0 );
    virtualPlayer(b2World &world, sf::Texture &Player_texture,sf::SoundBuffer&,int x, int y);

private:
    int sinhrCount = 0;
    int sinhrLim = 0;
};


#endif //GAME_VIRTUALPLAYER_H
