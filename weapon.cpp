/*
 * weapon.cpp
 *
 *  Created on: 3 ���. 2019 �.
 *      Author: ������
 */

#include "weapon.h"

weapon::weapon(b2World& world) :
		world(world) {
	std::string Path_to_duck =
			"/home/leonid/CLionProjects/Game/Resourses/";
	sf::Texture texture_weapon;
	if (!texture_weapon.loadFromFile(Path_to_duck + "box1.png")) {
		printf("FAILURE\n");
	}

	sprite.setTexture(texture_weapon);
	sprite.scale(0.07, 0.07);
	bound.setSize(
			sf::Vector2f(texture_weapon.getSize().x,
					texture_weapon.getSize().y));
	bound.setPosition(500, 100);
	moveable = true;
	bodyInit(world);

	bullet_position = realBody->GetPosition();
	bullet_angle = RayAngle;
}

void weapon::strike() {
	if (RayAngle < PI / 2 - (2 * recoil)) {
		is_strike = 1;
		RayAngle += 2 * recoil; //power of recoil
	}
}

void weapon::weapon_update() {

	b2RayCastInput input;
	b2RayCastOutput output;
	input.p1 = realBody->GetPosition();
	input.p2 = realBody->GetPosition()
			+ range * b2Vec2(cosf(RayAngle), sinf(RayAngle));
	input.maxFraction = 1;
	bool strike = 0;
	if (is_strike) {
		for (b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
				if (!f->RayCast(&output, input, 0)) {
					//std::cout << "miss at angle" << RayAngle * 360 / (2 * 3.141592) << std::endl;
					continue;
				} else {
					b2Vec2 target = b->GetPosition();
					if (1) {
						printf("STRIKE\n");
						strike = 1;
						bullet_position = realBody->GetPosition();
						break;
					}
				}
			}
			if (strike) {
				break;
			}
		}

		is_strike = 0;
		realBody->SetTransform(realBody->GetPosition(), RayAngle);
		if (!strike) {
			bullet_position.x += (1.0 / 60.0) * velocity * cosf(bullet_angle);
			bullet_position.y += (1.0 / 60.0) * velocity * sinf(bullet_angle);
		}
	}
	if (RayAngle > 0) {
		RayAngle -= recoil;
		realBody->SetTransform(realBody->GetPosition(), RayAngle); //power of recoil
	}
	update();
}
