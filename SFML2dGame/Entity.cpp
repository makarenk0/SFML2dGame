#include "Entity.h"



Entity::Entity(std::string textureName, TileMap* map, GameDataRef data, int entityHealth) : _map(map), _data(data), _entityHealth(entityHealth)
{

	x = 594;
	y = 201;
	w = PLAYER_TEXTURE_WIDTH;
	h = PLAYER_TEXTURE_HEIGHT;

	
	entity.initEntity(x, y,  w, h);
	entity._entity.setTexture(_data->asset.getTexture(textureName));
}

void Entity::updateEntity(float dt) {
	
	updateGravity(dt);
	moveEntity(dx*dt, 0);
	interactMap(dt);
}

void Entity::updateGravity(float dt) {  //floor
	if (fall) {
		fallingTime += dt;

		dy = ACCELERATION * ((fallingTime  * fallingTime)-jumpAcc) / 2 * dt;
		if (dy > 15) {  // limiting acceleration during falling
			dy = 15;
		}
		moveEntity(0, dy);

	}
	fall = true;
	for (int j = y + h - dy; j <= y + h; j++) {


		for (int i = x+1; i < x + w; i++) {
			if (_map->checkCollisionOfPoint(i, j)) {
				_entityHealth += _map->getDamageFromObject();
				fall = false;
				setEntityPos(x, ((j) / tileSize) * tileSize - h + _map->bottomSideDistance);
				break;
			}
		}
		if (!fall) {
			break;
		}
	}
	if (fall == false) {
		fallingTime = 1;
		jumpAcc = 0;
	}
//	std::cout << std::endl;

	for (int j = x + 1; j < x + w; j++) {   //ceiling
		if (dy < 0 && _map->checkCollisionOfPoint(j, y)) {
			setEntityPos(x, (y / tileSize + 1) * tileSize);
			fallingTime = 1;
			jumpAcc = 1;
			break;
		}
	}
}

void Entity::interactMap(float dt) {   //rigth and left sides
	


	for (int i = y; i < y + h; i++) {      
		for (int j = x; j < x + w; j += w-1) {     
			if (_map->checkCollisionOfPoint(j, i)) {   
			
				if (dx > 0) {                                                      
					setEntityPos(((x + w) / tileSize) * tileSize - w+ _map->rightSideDistance, y);
					dx = 0;
				}
				else if (dx < 0) {
					
					setEntityPos((x / tileSize) * tileSize +_map->leftSideDistance, y);
					dx = 0;
				}
				break;
			}
		}
	}


	
	
	

	
}

void Entity::drawEntity() {
	_data->window.draw(entity._entity);
}

void Entity::moveEntity(int dx, int dy) {
	entity.move(dx, dy);
	x += dx;
	y += dy;
}

void Entity::setEntityPos(int x, int y) {
	entity.setPosition(x, y);
	this->x = x;
	this->y = y;
}

