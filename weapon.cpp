


#include "weapon.h"
#include "player.h"

bullet::bullet() {
}

bullet::bullet(b2Vec2 r, float angle) :
		r(r), angle(angle) {
}

weapon::weapon(b2World& world, sf::RenderWindow& window, sf::Texture& texture_weapon,
		sf::Texture& texture_bullet, sf::Texture& texture_explosion, sf::SoundBuffer& shot_buffer,int x, int y) :
		world(world), window(window), texture_bullet(texture_bullet), texture_explosion(texture_explosion)
{
	shot_sound.setBuffer(shot_buffer);
	shot_sound.setVolume(100.f);

	sprite.setTexture(texture_weapon);
	sprite.scale(2.5, 2.5);
	bound.setSize(sf::Vector2f(texture_weapon.getSize().x,
							texture_weapon.getSize().y));
	bound.setPosition(x, y);
	movable = true;
	weapon_class = FireWeapon;
	bodyInit(world);
}

void weapon::strike() {
	if (flag && time >= (rapidity_of_fire)) {
		is_strike = true;
		++bullets_count;
		time = 0;
		shot_sound.play();
	}
}

void weapon::weapon_update() {
	if(abs(RayAngle) > PI/2 && direction == 1)
	{
		RayAngle = -PI + RayAngle;
	}
	else if(abs(RayAngle) < PI/2 && direction == -1)
	{
		RayAngle = PI - RayAngle;
	}

	if (is_strike) {

		b2Vec2 position;
		if(direction == 1)
		{
			position.x = realBody->GetPosition().x +
						(-bound.getSize().y * sinf(RayAngle) + bound.getSize().x * cosf(RayAngle)) / (2 * 40);
			position.y = realBody->GetPosition().y +
						(bound.getSize().y * cosf(RayAngle) + bound.getSize().x * sinf(RayAngle)) / (2 * 40);

		}
		else if(direction == -1)
		{
			position.x = realBody->GetPosition().x +
					(bound.getSize().y * sinf(PI - RayAngle) - bound.getSize().x * cosf(PI - RayAngle)) / (2 * 40);

			position.y = realBody->GetPosition().y +
					(bound.getSize().y * cosf(PI - RayAngle) + bound.getSize().x * sinf(PI - RayAngle)) / (2 * 40);
		}
		bullets[free_bullet] = bullet(position, RayAngle);
		sf::Sprite explosion_sprite;
		explosion_sprite.setTexture(texture_explosion, true);
		explosion_sprite.setPosition(sf::Vector2f(position.x * (40), position.y * (-40)));
		sf::IntRect rect = explosion_sprite.getTextureRect();
		explosion_sprite.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width/3, rect.height));
		explosion_sprite.setRotation(-RayAngle * (180 / PI));
		//40 is sqale ratio
		explosion_sprite.scale(1.8, 1.8);
		window.draw(explosion_sprite);

		realBody->ApplyLinearImpulseToCenter(b2Vec2(-line_recoil*cosf(RayAngle),-line_recoil*sinf(RayAngle)),true);
		if(abs(RayAngle) < PI/6 && direction == 1)
		{
			RayAngle += angle_recoil * pow((int)-1, int(bullets_count) % 2); //power of recoil
		}
		else if(PI - PI/6 < abs(RayAngle) &&
				abs(RayAngle) < PI + PI/6 && direction == -1)
		{
			RayAngle += angle_recoil * pow((int)-1, int(bullets_count) % 2);
		}
		realBody->SetTransform(realBody->GetPosition(), RayAngle);

	} else {
		if (RayAngle > 0 && direction == 1) {
			RayAngle -= angle_recoil / 10;
			if(RayAngle < 0)
			{
				RayAngle = 0;
			}
		}
		else if (RayAngle < 0 && direction == 1) {
			RayAngle += angle_recoil / 10;
			if(RayAngle > 0)
			{
				RayAngle = 0;
			}
		}
		else if (RayAngle > PI && direction == -1) {
			RayAngle -= angle_recoil / 10;
			if(RayAngle < PI)
			{
				RayAngle = PI;
			}
		}
		else if (RayAngle < PI && direction == -1) {
			RayAngle += angle_recoil / 10;
			if(RayAngle > PI)
			{
				RayAngle = PI;
			}
		}
		realBody->SetTransform(realBody->GetPosition(), RayAngle); //power of recoil
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
					continue;
				} else {
					b2Vec2 target = bullet.second.r + output.fraction * (range * b2Vec2(cosf(bullet.second.angle),
							sinf(bullet.second.angle)));

					if (std::abs(bullet.second.r.x - target.x) <= 0.5//0.5 is distance could be considered as a strike
							&& std::abs(bullet.second.r.y - target.y) <= 0.5) {
					    b->ApplyLinearImpulseToCenter(b2Vec2
					    		(line_recoil * cosf(bullet.second.angle), line_recoil * sinf(bullet.second.angle)), true);
					    sf::Sprite explosion_sprite;
					    explosion_sprite.setTexture(texture_explosion);
					    explosion_sprite.setPosition(sf::Vector2f(bullet.second.r.x * (40), bullet.second.r.y * (-40)));
					    explosion_sprite.setRotation(-bullet.second.angle * (180 / PI)); //???!!!
					    explosion_sprite.scale(1.8, 1.8);
					    //40 is sqale ratio
						//printf("STRIKE\n");
						used_bullets.push_back(bullet.first);
						explosion_sprites.push_back(std::pair<sf::Sprite, int>(explosion_sprite, 0));
						object* obj = static_cast<object *>(b->GetUserData());

						if(b != realBody &&(obj!= nullptr) && obj->movable)
						{
							object * obj = static_cast<object *>(b->GetUserData());
							if (obj->isPlayer){
								player * playerA = static_cast<class player *>(obj);
								playerA->death(200/40.0,50.0/-40.0);
							}
							b->SetTransform(b2Vec2(200.0 / 40.0, 50.0 / -40.0), 0);


							//default location and check whether object is ourself or map
						}
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
		//40 is sqale ratio

		bullet_sprite.rotate(-bullet.second.angle * (180 / PI));
		bullet_sprite.scale(1.25, 1.25);

		window.draw(bullet_sprite);

		bullet.second.r.x += (1.0 / 60.0) * velocity
				* cosf(bullet.second.angle);
		bullet.second.r.y += (1.0 / 60.0) * velocity
				* sinf(bullet.second.angle);
		if (std::abs(bullet.second.r.x) > xlim
				|| std::abs(bullet.second.r.y) > ylim) {
			used_bullets.push_back(bullet.first); //what if a bullet get there twice??? erase should return false
		}
	}
	for (auto& bullet : used_bullets) {
		bullets.erase(bullet);
		--bullets_count;
	}
	if (!used_bullets.empty()) {
		free_bullet = used_bullets[0];
		flag = true;
	} else if (bullets_count >= max_num_of_bullets || time >= 1) {
		flag = false;
	} else {
		flag = true;
		for (unsigned i = 1; i < max_num_of_bullets + 1; ++i) {
			if (bullets.count(i) == 0) {
				free_bullet = i;
				break;
			}
		}
	}
	used_bullets.clear();
	is_strike = 0;
	if(time < rapidity_of_fire)
	{
		time += (1/60.0);
	}
	auto iter = explosion_sprites.begin();
	while(iter != explosion_sprites.end())
	{
		sf::IntRect rect = (*iter).first.getTextureRect();
		(*iter).first.setTextureRect(sf::IntRect(rect.left + (*iter).second * rect.width/3,
				rect.top, rect.width/3, rect.height));
		(*iter).second += 1;
		if((*iter).second >= 3)
		{
			explosion_sprites.erase(iter);
		}
		else
		{
			window.draw((*iter).first);
			++iter;
		}
	}
	update();
	window.draw(sprite);
}
