//
// Created by leonid on 06.03.19.
//

#include <iostream>
#include "handWeapon.h"
#include "player.h"
#include <random>


handWeapon::handWeapon(b2World &world, sf::Texture &Texture, sf::Texture &TextureFire, sf::SoundBuffer& fire_buffer,int x, int y):
		flameTexture(TextureFire)
{
	fire_sound.setBuffer(fire_buffer);
	fire_sound.setVolume(100.f);
	fire_sound.setLoop(true);

    ancorPointShiftBodyAX = 20;
    ancorPointShiftBodyAY = 0;
    ancorPointShiftBodyBX = -10;
    ancorPointShiftBodyBY = 0;

    weapon_class = HandWeapon;
    bound.setPosition(x, y);
    sprite.setTexture(Texture);
    sprite.scale(1, 1);
    movable = true;
    dencity = 0.3;
    bodyInit(world,dencity);

    b2Fixture *fix =  realBody->GetFixtureList();
    b2Filter fil;
    fil.categoryBits = 6;
    fil.maskBits = 1;
    fix->SetFilterData(fil);
    fix->SetDensity(dencity);
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

void handWeapon::update(sf::RenderWindow &window)
{

	if(isBeingCaried && flag)
	{
		realBody->SetTransform(realBody->GetPosition(), PI2 / 4);
		flag = false;
	}
	else if(!isBeingCaried)
	{
		flag = true;
	}

	if((direction == 1 && realBody->GetAngle() > PI2/2) ||
			(direction == -1 && realBody->GetAngle() < PI2/2))
	{
		realBody->SetTransform(realBody->GetPosition(), PI2 - realBody->GetAngle());
	}

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


    if ((isBeingCaried && realBody->GetAngle() <= 0 && direction == 1) ||
    		(isBeingCaried && realBody->GetAngle() >= PI2 && direction == -1)) {
        //Go up
        goUp = true;
        goDown = false;
        realBody->SetAngularVelocity(direction * angularVelocity);
    }
    if (goUp) {

        if ((realBody->GetAngle() > PI2 / 4 && direction == 1)
        		|| (realBody->GetAngle() < PI2 - PI2 / 4 && direction == -1)) {
            goUp = false;
            realBody->SetAngularVelocity(0);
            if(direction == 1)
            {
            	realBody->SetTransform(realBody->GetPosition(), PI2 / 4);
            }
            else if(direction == -1)
            {
            	realBody->SetTransform(realBody->GetPosition(), PI2 - PI2 / 4);
            }
        }

    }
    if (goDown && isBeingCaried)
    {
        //realBody->SetTransform(realBody->GetPosition(), realBody->GetAngle() - delta);
       /* if ((realBody->GetAngle() < 0 && direction == 1)
        	|| (realBody->GetAngle() > PI2 && direction == -1)) {

            goUp = true;
            goDown = false;
            realBody->SetAngularVelocity(0);
        }*/


           if(!reachableObjects.empty()){
               std::set<object *>::iterator iter3;
               for (iter3 = reachableObjects.begin();iter3 != reachableObjects.end();iter3++) {
                   object * ContactObject = *iter3;
                   if(ContactObject->isPlayer){
                       player* pl = dynamic_cast<player*>(ContactObject);
                       pl->death(0,0);
                   }

                   std::random_device rd;  //Will be used to obtain a seed for the random number engine
                   std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
                   std::uniform_int_distribution<> dis(-60, -10);
                   makeSparkels(ContactObject->bound.getPosition().x + dis(gen),
                                ContactObject->bound.getPosition().y + dis(gen));
               }
           }
    }

  	if(fire_sound.getStatus() != sf::Sound::Playing && !FLameOrigin.empty())
    {
    	fire_sound.play();
    }
  	else if(FLameOrigin.empty())
  	{
  		fire_sound.stop();
  	}

    object::update();
    window.draw(sprite);

    time_delay += 1 / 60.0;
    if(time_delay > delay)
    {
    	time_delay = delay;
    }

}


void handWeapon::strike() {
    	if(time_delay < delay)
    	{
    		return;
    	}
        realBody->SetAngularVelocity(-1 * direction * angularVelocity);
        goDown = true;
        time_delay = 0;
}
/*  if ((realBody->GetAngle() >= PI2 / 4 && direction == 1) ||
    		(realBody->GetAngle() >= PI2 - PI2 / 4 && direction == -1)) { */

