/*
 * weapon.cpp
 *
 *  Created on: 3 ���. 2019 �.
 *      Author: ������
 */

#include "weapon.h"

bullet::bullet() {
}

bullet::bullet(b2Vec2 r, float angle) :
		r(r), angle(angle) {
}

weapon::weapon(b2World& world, sf::RenderWindow& window, sf::Texture& texture_weapon, sf::Texture& texture_bullet) :
		world(world), window(window), texture_bullet(texture_bullet) {
	isThisFireWeapon=true;
	sprite.setTexture(texture_weapon);
	sprite.scale(2.5, 2.5);
	bound.setSize(sf::Vector2f(texture_weapon.getSize().x,
							texture_weapon.getSize().y));
	bound.setPosition(500, 100);
	moveable = true;
	bodyInit(world);


}

void weapon::strike() {
	if (RayAngle < PI / 2 - (angle_recoil) && flag) {
		is_strike = true;
		++bullets_count;
	}
}

void weapon::weapon_update() {

	if (is_strike) {
		b2Vec2 position(realBody->GetPosition().x +
				(bound.getSize().x * cosf(RayAngle) - bound.getSize().y * sinf(RayAngle)) / (2 * 40),
		realBody->GetPosition().y +
		(bound.getSize().x * sinf(RayAngle) + bound.getSize().y * cosf(RayAngle)) / (2 * 40));
		bullets[free_bullet] = bullet(position, RayAngle);
		RayAngle += angle_recoil; //power of recoil;
		int modul = 10;
		realBody->ApplyLinearImpulseToCenter(b2Vec2(-modul*cosf(RayAngle),-modul*sinf(RayAngle)),true);
		realBody->SetTransform(realBody->GetPosition(), RayAngle);

	} else {
		if (RayAngle > 0) {
			RayAngle -= angle_recoil / 2;
			if (RayAngle < 0) {
				RayAngle = 0;
			}
			realBody->SetTransform(realBody->GetPosition(), RayAngle); //power of recoil
		}
	}
	bool shot = false;
	for (auto& bullet : bullets) {
		b2RayCastInput input;
		b2RayCastOutput output;
		input.p1 = bullet.second.r;
		input.p2 = bullet.second.r
				+ range
						* b2Vec2(cosf(bullet.second.angle),
								sinf(bullet.second.angle));
		input.maxFraction = 1;
		for (b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
				if (!f->RayCast(&output, input, 0)) {
					//std::cout << "miss at angle" << RayAngle * 360 / (2 * 3.141592) << std::endl;
					continue;
				} else {
					b2Vec2 target = bullet.second.r + output.fraction * (range * b2Vec2(cosf(bullet.second.angle),
							sinf(bullet.second.angle)));

					//printf("%f, %f : %f, %f \n", bullet.second.r.x, bullet.second.r.y, target.x, target.y);
					if (std::abs(bullet.second.r.x - target.x) <= 0.5
							&& std::abs(bullet.second.r.y - target.y) <= 0.5) {
						//printf("STRIKE\n");
						used_bullets.push_back(bullet.first);
						shot = true;
						break;
					}
				}
			}
			if (shot) {
				shot = false;
				break;
			}
		}
	}
	for (auto& bullet : bullets) {

		sf::Sprite bullet_sprite;
		bullet_sprite.setTexture(texture_bullet);
		bullet_sprite.setPosition(sf::Vector2f(bullet.second.r.x * (40), bullet.second.r.y * (-40)));
		//40 is sqale ratio 10 is g parametr
		bullet_sprite.rotate(-bullet.second.angle * (180 / PI));
		bullet_sprite.scale(2, 2);

		window.draw(bullet_sprite);

		bullet.second.r.x += (1.0 / 60.0) * velocity
				* cosf(bullet.second.angle);
		bullet.second.r.y += (1.0 / 60.0) * velocity
				* sinf(bullet.second.angle);
		if (std::abs(bullet.second.r.x) > xlim
				|| std::abs(bullet.second.r.y) > ylim) {
			used_bullets.push_back(bullet.first);
		}
	}
	//printf("%d\n", bullets_count);
	for (auto& bullet : used_bullets) {
		bullets.erase(bullet);
		--bullets_count;
	}
	if (!used_bullets.empty()) {
		free_bullet = used_bullets[0];
		flag = true;
	} else if (bullets_count == 5) {
		flag = false;
	} else {
		flag = true;
		for (unsigned i = 1; i < 6; ++i) {
			if (bullets.count(i) == 0) {
				free_bullet = i;
				break;
			}
		}
	}
	used_bullets.clear();
	is_strike = 0;
	update();
}
