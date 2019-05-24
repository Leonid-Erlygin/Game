/*
 * grenade.h
 *
 *  Created on: 8 ���. 2019 �.
 *      Author: ������
 */

#pragma once
#include "object.h"
#include <vector>


#ifndef GRENADE_H_
#define GRENADE_H_

class QueryCallback : public b2QueryCallback {
public:
	std::vector<b2Body*> foundBodies;
    bool ReportFixture(b2Fixture*);
};

class grenade : public object {
public:


	b2World& world;
	sf::RenderWindow& window;
	grenade(b2World& world, sf::RenderWindow&, sf::Texture&, sf::Texture&, sf::SoundBuffer&,int,int );
	void explode();
	void strike();
	void grenade_update();
	float time = 0;
	bool is_thrown = 0;
	std::vector<std::pair<sf::Sprite, int>> explosion_sprites;

private:
	sf::Sound explosion_sound;

	sf::Texture& grenade_texture;
	sf::Texture& explosion_texture;

	int range = 8;
	float explosion_delay = 1.0;

};




#endif /* GRENADE_H_ */
