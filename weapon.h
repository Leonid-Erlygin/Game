/*
 * weapon.h
 *
 *  Created on: 3 мар. 2019 г.
 *      Author: Никита
 */
#pragma once
#include "object.h"
#include <map>
#include <tuple>
#include <vector>
#include <cmath>
#define PI 3.141593

#ifndef WEAPON_H_
#define WEAPON_H_

struct bullet
{
	bullet();
	bullet(b2Vec2 r, float angle);
	b2Vec2 r;
	float angle;
};


class weapon : public object {
public:

	sf::RenderWindow& window;
	b2World& world;

	weapon(b2World& world, sf::RenderWindow&, sf::Texture&, sf::Texture&, sf::Texture&);
	void strike();
	void weapon_update();

	float RayAngle = 0;

	std::map<unsigned, bullet> bullets;
	std::vector<unsigned> used_bullets;
	std::vector<std::pair<sf::Sprite, int>> explosion_sprites;

	unsigned free_bullet = 1;
	unsigned bullets_count = 0;
	float time = rapidity_of_fire;

	bool flag = true;
	bool is_strike = 0;

private:

	sf::Texture& texture_explosion;
	sf::Texture& texture_bullet;
	unsigned max_num_of_bullets = 100;
	int damage = 10;
	int range = 50; //max distance for a bullet to fly
	int xlim = 50; //bounds a bullet can't go through
	int ylim = 50;
	float rapidity_of_fire = 0.07; //time between shots
	float velocity = 25;
	float angle_recoil = 10 * (PI / 180); //the angle to be diflected at after strike
	float line_recoil = 5; //the distance to be thrown at after strike
};

#endif /* WEAPON_H_ */
