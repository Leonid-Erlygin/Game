//
// Created by leonid on 06.03.19.
//

#include <iostream>
#include "handWeapon.h"
#include <random>


void handWeapon::switchToRigth() {
    ancorPointShiftBodyAX = 20;
    ancorPointShiftBodyAY = 0;
    ancorPointShiftBodyBX = -10;
    ancorPointShiftBodyBY = 0;

}


handWeapon::handWeapon(b2World &world, sf::Texture &Texture, sf::Texture &TextureFire) : flameTexture(TextureFire) {

    ancorPointShiftBodyAX = 20;
    ancorPointShiftBodyAY = 0;
    ancorPointShiftBodyBX = -10;
    ancorPointShiftBodyBY = 0;
    weapon_class = HandWeapon;
    bound.setPosition(600, 40);
    sprite.setTexture(Texture);
    sprite.scale(1, 1);
    moveable = true;
    bodyInit(world);

    b2Fixture *fix =  realBody->GetFixtureList();
    b2Filter fil;
    fil.categoryBits = 6;
    fil.maskBits = 1;
    fix->SetFilterData(fil);
    fix->SetDensity(10);
    b2FixtureDef fixture2;
    b2PolygonShape shape;

    shape.SetAsBox((bound.getSize().x / (float32) (2 * scale_factorX)) * bound.getScale().x,
                   (bound.getSize().y / (float32) (2 * scale_factorX)) * bound.getScale().y);

    fixture2.shape = &shape;
    fixture2.density = 0;
    fixture2.isSensor = true;
    realBody->CreateFixture(&fixture2);


}

void handWeapon::makeSparkels(float x, float y) {
    int col[6] = {0, 43, 83, 125, 167, 205};
    int row[4] = {0, 60, 132, 198};

    std::vector<sf::Sprite> fireSprites1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            sf::Sprite sprite1;
            if (i == 0) sprite1.setPosition(x, y + 10);
            else sprite1.setPosition(x, y);
            sprite1.setTexture(flameTexture);
            sprite1.setTextureRect(sf::IntRect(col[j], row[i], col[j + 1] - col[j], row[i + 1] - row[i]));
            fireSprites1.push_back(sprite1);
        }

    }
    FLameOrigin.push_back(fireSprites1);
    FLameState.push_back(0);
}

void handWeapon::update(sf::RenderWindow &window) {

    std::vector<std::vector<sf::Sprite>>::iterator iter;
    std::vector<int>::iterator iter2;
    iter2 = FLameState.begin();
    int counterDot = 0;

    for (iter = FLameOrigin.begin(); iter < FLameOrigin.end(); iter++) {
            window.draw(FLameOrigin[counterDot][FLameState[counterDot]]);


        if(flameDilayCount == 5)FLameState[counterDot]++;
        if (FLameState[counterDot] > 14) {
            FLameOrigin.erase(iter);
            FLameState.erase(iter2);
        }
        flameDilayCount++;
        if(flameDilayCount>flameDilay)flameDilayCount = 0;
        counterDot++;
        iter2++;

    }


    if (isInHands && realBody->GetAngle() <= 0) {
        //Go up
        goUp = true;
        realBody->SetAngularVelocity(angularVelosity);
    }
    if (goUp) {

        if (realBody->GetAngle() > PI2 / 4) {
            goUp = false;
            realBody->SetAngularVelocity(0);
        }

    }
    if (goDown) {
        //realBody->SetTransform(realBody->GetPosition(), realBody->GetAngle() - delta);
        if (realBody->GetAngle() < 0) {

            goUp = true;
            goDown = false;
            realBody->SetAngularVelocity(0);
        }


           if(!reachableObjects.empty()){
               std::set<object *>::iterator iter3;
               for (iter3 = reachableObjects.begin();iter3 != reachableObjects.end();iter3++) {
                   object * ContactObject = *iter3;
                   std::random_device rd;  //Will be used to obtain a seed for the random number engine
                   std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
                   std::uniform_int_distribution<> dis(-60, -10);
                   makeSparkels(ContactObject->bound.getPosition().x + dis(gen),
                                ContactObject->bound.getPosition().y + dis(gen));
               }
           }




    }

    object::update();

}


void handWeapon::strike() {
    if (realBody->GetAngle()>0&&realBody->GetAngle() > PI2 / 4) {
        realBody->SetAngularVelocity(-angularVelosity);

        goDown = true;
    }
}


