/*
 * weapon.h
 *
 *  Created on: 3 ���. 2019 �.
 *      Author: ������
 */
#pragma once
#include "object.h"
#define PI 3.141593

#ifndef WEAPON_H_
#define WEAPON_H_


class weapon : public object {
public:

	weapon(b2World& world);
	void strike();
	void weapon_update();
	unsigned time_to_strike();

	double RayAngle = 0;
	b2World& world;
	b2Vec2 bullet_position;
	double bullet_angle;
	bool is_strike = 0;

private:

	int damage = 10;
	int range = 2000000;
	double velocity = 1000;
	double recoil = 10 * (PI / 180); //an angle to diflect weapon of one shot
};

#endif /* WEAPON_H_ */
