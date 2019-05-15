//
// Created by leonid on 06.03.19.
//

#ifndef GAME_HANDWEAPON_H
#define GAME_HANDWEAPON_H


#include <set>
#include "object.h"

class handWeapon : public object {
public:
    handWeapon(b2World &, sf::Texture &, sf::Texture &, sf::SoundBuffer&,int, int );

    int angle = 0;
    bool goUp = false;
    bool flag = true;
    void switchToRigth();
    void switchToLeft();
    std::set<object *>reachableObjects;

    void strike();

    void makeSparkels(float x, float y);

    std::vector<sf::Sprite> fireSprites;

    void update(sf::RenderWindow&);

    float time_delay = delay;

private:

    sf::Sound fire_sound;

    sf::Texture &flameTexture;
    std::vector<std::vector<sf::Sprite>> FLameOrigin;
    std::vector<int> FLameState;
    int flameDilay = 5;
    int flameDilayCount = 0;
    int ancorPointShiftX = 40;
    int ancorPointShiftY = -20;
    bool goDown = false;
    int anglePartition = 20;
    int angularVelocity = 20;
    float PI2 = 3.14159265f;
    float delta = 3.14159265f / (4 * anglePartition);
    float32 scale_factorX = 40;//1 м = 40 pxl
    float32 scale_factorY = -40;
    float delay = 0.25;


    bool help = true;

};


#endif //GAME_HANDWEAPON_H
