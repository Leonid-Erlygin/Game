/*
 * grenade.cpp
 *
 *  Created on: 8 ���. 2019 �.
 *      Author: ������
 */


#include "grenade.h"
#include "player.h"

bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
	foundBodies.push_back(fixture->GetBody());
    return true;//keep going to find all fixtures in the query area
};

grenade::grenade(b2World& world, sf::RenderWindow& window,
		sf::Texture& grenade_texture, sf::Texture& explosion_texture) : world(world), window(window),
		grenade_texture(grenade_texture), explosion_texture(explosion_texture)
{
	sprite.setTexture(grenade_texture);
	sf::IntRect rect = sprite.getTextureRect();
	sprite.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width/2, rect.height));
	sprite.scale(2.5, 2.5);
	bound.setSize(sf::Vector2f(grenade_texture.getSize().x,
								grenade_texture.getSize().y));
	bound.setPosition(200, 50);
	movable = true;
	weapon_class = Grenade;
	bodyInit(world);

}

void grenade::explode()
{
	QueryCallback callback;
	b2AABB aabb;
	b2Vec2 position = realBody->GetPosition();
	b2Vec2 lb(position.x - range/2, position.y - range/2);
	b2Vec2 ub(position.x + range/2, position.y + range/2);
	aabb.lowerBound = lb;
	aabb.upperBound = ub;
	world.QueryAABB(&callback, aabb);
	for(b2Body* damaged_object : callback.foundBodies)
	{
		sf::Sprite strike_sprite;
		strike_sprite.setTexture(explosion_texture);
		strike_sprite.setPosition(sf::Vector2f(damaged_object->GetPosition().x * (40),
				damaged_object->GetPosition().y * (-40)));
		//40 is sqale ratio
		strike_sprite.scale(2, 2);
		explosion_sprites.push_back(std::pair<sf::Sprite, int>(strike_sprite, 0));
		object* obj = static_cast<object *>(damaged_object->GetUserData());

		if(obj!= nullptr&&obj->movable)
		{
			if (obj->isPlayer){
				player * playerA = static_cast<class player *>(obj);
				playerA->death(200/40.0,50.0/-40.0);
				if(isBeingCariedBy!= nullptr){
					player * playerB =static_cast<player *> (isBeingCariedBy->GetUserData());
					object * obj2 = dynamic_cast<object *>(this);
					playerB->reachableObjects.erase(obj2);
				}

			}

			damaged_object->SetTransform(b2Vec2(200.0 / 40.0, 50.0 / -40.0), 0);

			//default location and check whether object is ourself or map

		}
	}
	sf::Sprite strike_sprite;
	strike_sprite.setTexture(explosion_texture);
	strike_sprite.setPosition(sf::Vector2f(realBody->GetPosition().x * (40),
			realBody->GetPosition().y * (-40)));
	//40 is sqale ratio
	strike_sprite.scale(2, 2);
	explosion_sprites.push_back(std::pair<sf::Sprite, int>(strike_sprite, 0));
	time = 0;
	is_thrown = 0;
	//probably person Carying the object could switch



	//f!!!!
	if (isBeingCaried){

		player * playerB =static_cast<player *> (isBeingCariedBy->GetUserData());
		object * obj = dynamic_cast<object *>(this);
		playerB->reachableObjects.erase(obj);
	}





	world.DestroyBody(realBody);
	realBody = nullptr; //but this shit still can be taken!!!
}

void grenade::strike()
{
	if(is_thrown == 1)
	{
		return;
	}
	is_thrown = 1;
	sf::IntRect rect = sprite.getTextureRect();
	sprite.setTextureRect(sf::IntRect(rect.left + 4 * rect.width / 5, rect.top, rect.width, rect.height));
	//4, 5 params depends on texture

}

void grenade::grenade_update()
{
	if(is_thrown == 1)
	{
		time += (1.0 / 60.0);
		if(time >= explosion_delay)
		{
			explode();
			return;
		}
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
	if(realBody != nullptr)
	{
		update();
		window.draw(sprite);
	}
}
