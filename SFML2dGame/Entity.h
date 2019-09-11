#pragma once
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"
#include "TileMap.h"
#include "Game.h"
#include <iostream>
class Entity
{
public:
	Entity(std::string textureName, TileMap* map, GameDataRef data, int entityHelth);
	void drawEntity();
	int x, y;
protected:
	void updateEntity(float dt);
	void moveEntity(int dx, int dy);
	void setEntityPos(int x, int y);
	int w, h, _speed, dir;
	float dx, dy, fallingTime = 0, jumpAcc=0;
	bool fall = false;
	int _entityHealth;

	struct EntityData {
		sf::Sprite _entity;
		sf::IntRect _entityHitBox;
		void initEntity(int x, int y, int width, int height) {
			_entity.setTextureRect(sf::IntRect(0,0, width, height));
			_entityHitBox.width = width;
			_entityHitBox.height = height;
			this->setPosition(x, y);
		}
		void move(float dx, float dy) {
			_entity.move(dx, dy);
			_entityHitBox.left += dx;
			_entityHitBox.top += dy;
		}
		void setPosition(float x, float y) {
			_entity.setPosition(x, y);
			_entityHitBox.left = x;
			_entityHitBox.top = y;
		}
		bool intersects(sf::IntRect rect) {
			if (_entityHitBox.intersects(rect)) {
				return true;
			}
			return false;
		}

	};

	EntityData entity;
	GameDataRef _data;
	TileMap* _map;
private:
	
	
	
	void interactMap(float dt);
	void updateGravity(float dt);
};

