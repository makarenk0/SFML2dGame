#include "TileMap.h"



TileMap::TileMap(GameDataRef data, int visibleWidth, int visibleHeight, std::string mapName, bool campaignMode) : BaseMap(data)
{
	_visibleWidth = visibleWidth;
	_visibleHeight = visibleHeight;
	
	initTilesHitboxes();
	srand(time(0));
	initMap(mapName, campaignMode);
}

void TileMap::initMap(std::string mapName, bool campaignMode) {
	std::ifstream file;
	if (campaignMode) {
		file.open("maps/campaign/" + mapName + ".txt");
	}
	else {
		file.open("maps/custom/" + mapName + ".txt");
	}
	
	std::string line;
	std::string value = "";
	int x, y, tileId, objectId, blockNum = 0;
	bool solid;

	//player start position
	std::getline(file, line);
	line = line.substr(7, line.length());
	readValue(line, value);
	this->playerStartPosX = std::stoi(value);
	readValue(line, value);
	this->playerStartPosY = std::stoi(value);
	value = "";
	//init canvas that fits tilemap
	std::getline(file, line);
	line = line.substr(8, line.length());
	readValue(line, value);
	mapWidth = std::stoi(value);
	readValue(line, value);
	mapHeight = std::stoi(value);
	_tilesInCol = mapHeight / tileSize;
	_tilesInRow = mapWidth / tileSize;

	canvas.create(mapWidth, mapHeight);
	idData.resize(_tilesInCol);
	canvas.clear();
	canvas.display();

	//--------------------------------
	backgroundCanvas.create(_visibleWidth, _tilesInCol*tileSize);
	background1.setTexture(_data->asset.getTexture("Background1"));
	background1.setPosition(0, _tilesInCol * tileSize / 2);
	background1.setScale(1, _tilesInCol * tileSize/2/ _data->asset.getTexture("Background1").getSize().y);
	backgroundCanvas.draw(background1);

	background2.setTexture(_data->asset.getTexture("Background2"));
	background2.setPosition(0, 0);
	background2.setScale(1, _tilesInCol * tileSize / 2 / _data->asset.getTexture("Background2").getSize().y);
	backgroundCanvas.draw(background2);
	backgroundCanvas.display();


	
	
	//-------------------------
	std::map<std::string, sf::VertexArray> block;
	initMapBlocks(block);


	for (int i=0;;i++) {
		value = "";
		std::getline(file, line);

		if (line == "endBlock") {
			map.push_back(block);
			initMapBlocks(block);
			i = -1;
			blockNum++;
			continue;
		}
			
		if (line == "end") break;

		//read all values in line
		readValue(line, value);
		x = std::stoi(value);
		
		readValue(line, value);
		y = std::stoi(value);

		readValue(line, value);
		tileId = std::stoi(value);
	    

		readValue(line, value);
		objectId = std::stoi(value);

		idData[i / MAP_BLOCK_WIDTH].push_back(sf::Vector2<short int>(tileId, objectId));
		//-----------------------
		sf::Vertex* vertexTile = &block["blockTiles"][i * 4.0];
		setTilePosition(vertexTile, x, y);
		setTileTextureCords(vertexTile, tileId%(_tileTextureWidth/tileSize), tileId/(_tileTextureWidth /tileSize));

		sf::Vertex* vertexObject = &block["blockObjects"][i * 4.0];
		setTilePosition(vertexObject, x, y);
		setTileTextureCords(vertexObject, objectId % (_tileTextureWidth / tileSize), objectId / (_tileTextureWidth / tileSize));
		
	}
	changeTile(playerStartPosX, playerStartPosY, 0, false);
	file.close();
	//test
	reDrawCanvas();
}

void TileMap::addTrigger(int x, int y, short int objectId) {
	if (objectId == 16) {
		//triggers.push_back(std::tuple<int, int, sf::IntRect>(objectId, 2, sf::IntRect(x * tileSize, y * tileSize, tileSize, tileSize)));
	}
	
}

bool TileMap::checkIfSameTrigger(int x, int y)
{
	x = (x / tileSize) * tileSize;
	y = (y / tileSize) * tileSize;
	for (auto& i : _triggers) {
		if (i->x == x && i->y == y) {
			return true;
		}
	}
	return false;
}

void TileMap::setTilePosition(sf::Vertex* vertexPtr, int x, int y) {
	vertexPtr[0].position = sf::Vector2f(x * tileSize, y * tileSize);
	vertexPtr[1].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize);
	vertexPtr[2].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
	vertexPtr[3].position = sf::Vector2f(x * tileSize, y * tileSize + tileSize);
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	_data->window.draw(background);
	_data->window.draw(mapSprite);
}

void TileMap::mapAnimationsUpdate(float dt, int dx, int x) {
	backgroundCanvas.clear();
	backgroundCanvas.draw(background2);
	backgroundCanvas.draw(background1);
	if (clock.getElapsedTime().asSeconds() > 5) {
		sf::Sprite cloud;
		cloud.setTexture(_data->asset.getTexture("Clouds"));
		cloud.setTextureRect(sf::IntRect(rand() % 4 * 166, 0, 166, 53));
		cloud.setPosition(_visibleWidth, rand() % _tilesInCol*tileSize / 3);
		clouds.push_back(cloud);
		clock.restart();
	}
		for (auto& i : clouds) {
			i.move(-CLOUDS_SPEED - dt*1.0 * dx, 0.f);
			backgroundCanvas.draw(i);
		}
		if (!clouds.empty()&&clouds.front().getPosition().x + 166 < 0) {
			clouds.pop_front();
		}
	background.setTexture(backgroundCanvas.getTexture());

	/*triggers update*/
	for (int i = 0; i < _triggers.size(); i++) {
		_triggers[i]->update();
		if (_triggers[i]->getOnUpdate()) {
			auto res = _triggers[i]->_updateData;
			changeTile(res[0], res[1], res[2], res[3]);
			idData[res[1]/tileSize][res[0]/tileSize].y = (short)res[2];
		}
		if (_triggers[i]->getOnRemove()) {
			delete _triggers[i];
			_triggers.erase(_triggers.begin() + i);
		}
		
	}
}

bool TileMap::checkCollisionOfPoint(int x, int y) {
	damageFromObject = 0;
	bottomSideDistance = 0;
	rightSideDistance = 0;
	leftSideDistance = 32;
	idBuf = idData[y / tileSize][x / tileSize];
	if (idBuf.y == 0) {
		int id = idBuf.x;
		if (id == 0 || (id>32&&id<36)) {
			return false;
		}
		if ((id > 0 && id < 21)) {
			
			bottomSideDistance = 0;
			leftSideDistance = 32;
			rightSideDistance = 0;
			return true;
		}
		else if (id > 20 && id < 27) {
			if (pointInPolygon(x, y, 0)) {
				computeOffsets(x%tileSize, y%tileSize, 0);
				rightSideDistance += 2;
				return true;
			}
		}
		else if (id > 26 && id < 33) {
			if (pointInPolygon(x, y, 1)) {
				computeOffsets(x % tileSize, y % tileSize, 1);
				leftSideDistance -= 2;
				return true;
			}
		}
		else if (id > 35) {
			bottomSideDistance = 0;
			leftSideDistance = 32;
			rightSideDistance = 0;
			return true;
		}
	}
	else {
		int id = idBuf.y;
		
		if (id == 1) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !checkIfSameTrigger(x, y)) {
				_triggers.push_back(new TriggerReact(std::tuple<int, int, sf::IntRect>(1, 4, sf::IntRect(x, y, tileSize, tileSize)), 0.5));
				_data->particles.addParticlesEffect(new CoinsParticlesSet((x/tileSize) * tileSize + 10, (y/tileSize) * tileSize + 2 * tileSize/3, 4000, 1000, _data->window));
			}
			if (pointInPolygon(x, y, 2)) {
				computeOffsets(x % tileSize, y % tileSize, 2);
				return true;
			}
		}
		else if (id == 2) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !checkIfSameTrigger(x, y)) {
				_triggers.push_back(new TriggerReact(std::tuple<int, int, sf::IntRect>(2, 4, sf::IntRect(x, y, tileSize, tileSize)), 0.5));
				_data->particles.addParticlesEffect(new CoinsParticlesSet((x / tileSize) * tileSize + 10, (y / tileSize) * tileSize + 2 * tileSize / 3, 4000, 1000, _data->window));
			}
			if (pointInPolygon(x, y, 2)) {
				computeOffsets(x % tileSize, y % tileSize, 2);
				return true;
			}
		}
		else if (id == 10) {
			if (pointInPolygon(x, y, 3)) {
				computeOffsets(x % tileSize, y % tileSize, 3);
				return true;
			}
		}
		else if (id > 20 && id < 24) {
			return true;
		}
		else if (id == 25) {
			if (pointInPolygon(x, y, 4)) {
				computeOffsets(x % tileSize, y % tileSize, 4);
				damageFromObject = -1;
				return true;
			}
		}
		else if (id == 26) {
			if (pointInPolygon(x, y, 5)) {
				computeOffsets(x % tileSize, y % tileSize, 5);
				damageFromObject = -2;
				return true;
			}
		}
		else if (id == 28) {
			playerWin = true;
		}
		else if (id == 33) {
			if (pointInPolygon(x, y, 6)) {
				computeOffsets(x % tileSize, y % tileSize, 6);
				return true;
			}
		}
		else if (id == 39 || id == 41 || id == 43 || id == 47) {
			if (!checkIfSameTrigger(x, y)) {
				_triggers.push_back(new TriggerReact(std::tuple<int, int, sf::IntRect>(id - 2, 2, sf::IntRect(x, y, tileSize, tileSize)), 0));
				_playerInventory->putInFreeSlot(id-2);
			}
		}
		else if (id == 40 || id == 42 || id == 44 || id == 48) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !checkIfSameTrigger(x, y) && _playerInventory->isSuchItem(id - 3)) {
				_triggers.push_back(new TriggerReact(std::tuple<int, int, sf::IntRect>(id - 2, 4, sf::IntRect(x, y, tileSize, tileSize)), 0.5));
				_playerInventory->removeItem(id - 3);
			}
			if (pointInPolygon(x, y, 7)) {
				computeOffsets(x % tileSize, y % tileSize, 7);
				return true;
			}
		}
		else if (id % _objectTextureWidthElements == 1 || id % _objectTextureWidthElements == 2) {
			if (pointInPolygon(x, y, 2)) {
				computeOffsets(x % tileSize, y % tileSize, 2);
				return true;
			}
		}
		else{
			return false;
		}
		
		
	}

}

int TileMap::getDamageFromObject() {
	return damageFromObject;
}

void TileMap::reDrawCanvas()
{
	canvas.clear(sf::Color::Transparent);
	BaseMap::reDrawCanvas();
}

void TileMap::initTilesHitboxes() {
	sf::ConvexShape hitbox;
	


	hitbox.setPointCount(10);  //stairs 0
	hitbox.setPoint(0, sf::Vector2f(0, tileSize));
	hitbox.setPoint(1, sf::Vector2f(0, 3 * tileSize / 4));
	hitbox.setPoint(2, sf::Vector2f(tileSize / 4, 3 * tileSize / 4));
	hitbox.setPoint(3, sf::Vector2f(tileSize / 4, 2 * tileSize / 4));
	hitbox.setPoint(4, sf::Vector2f(2*tileSize / 4 , 2 * tileSize / 4));
	hitbox.setPoint(5, sf::Vector2f(2 * tileSize / 4, tileSize / 4));
	hitbox.setPoint(6, sf::Vector2f(3 * tileSize / 4, tileSize / 4));
	hitbox.setPoint(7, sf::Vector2f(3 * tileSize / 4, 0));
	hitbox.setPoint(8, sf::Vector2f(tileSize, 0));
	hitbox.setPoint(9, sf::Vector2f(tileSize, tileSize));
	
	tilesHitboxes.push_back(hitbox);
	

	//1
	hitbox.setPointCount(10);
	hitbox.setPoint(0, sf::Vector2f(0, 0));
	hitbox.setPoint(1, sf::Vector2f(tileSize/4, 0));
	hitbox.setPoint(2, sf::Vector2f(tileSize / 4, tileSize / 4));
	hitbox.setPoint(3, sf::Vector2f(2*tileSize / 4, tileSize / 4));
	hitbox.setPoint(4, sf::Vector2f(2 * tileSize / 4, 2 * tileSize / 4));
	hitbox.setPoint(5, sf::Vector2f(3 * tileSize / 4, 2* tileSize/4));
	hitbox.setPoint(6, sf::Vector2f(3 * tileSize / 4, 3* tileSize / 4));
	hitbox.setPoint(7, sf::Vector2f(tileSize, 3*tileSize/4));
	hitbox.setPoint(8, sf::Vector2f(tileSize, tileSize));
	hitbox.setPoint(9, sf::Vector2f(0, tileSize));
	tilesHitboxes.push_back(hitbox);

	//chest
	hitbox.setPointCount(4);
	hitbox.setPoint(0, sf::Vector2f(1, tileSize));
	hitbox.setPoint(1, sf::Vector2f(1, tileSize - 21));
	hitbox.setPoint(2, sf::Vector2f(tileSize - 1, tileSize - 21));
	hitbox.setPoint(3, sf::Vector2f(tileSize - 1, tileSize));
	tilesHitboxes.push_back(hitbox);

	//stone
	hitbox.setPointCount(6);
	hitbox.setPoint(0, sf::Vector2f(2, tileSize));
	hitbox.setPoint(1, sf::Vector2f(2, tileSize - 13));
	hitbox.setPoint(2, sf::Vector2f(tileSize - 10, tileSize - 13));
	hitbox.setPoint(3, sf::Vector2f(tileSize - 10, tileSize - 6));
	hitbox.setPoint(4, sf::Vector2f(tileSize-1, tileSize-6));
	hitbox.setPoint(5, sf::Vector2f(tileSize - 1, tileSize));
	tilesHitboxes.push_back(hitbox);

	//4
	
	hitbox.setPointCount(4);
	hitbox.setPoint(0, sf::Vector2f(8, tileSize));
	hitbox.setPoint(1, sf::Vector2f(8, tileSize - 12));
	hitbox.setPoint(2, sf::Vector2f(tileSize - 7, tileSize - 12));
	hitbox.setPoint(3, sf::Vector2f(tileSize - 7, tileSize));
	tilesHitboxes.push_back(hitbox);
	
	//5
	hitbox.setPointCount(4);
	hitbox.setPoint(0, sf::Vector2f(1, tileSize));
	hitbox.setPoint(1, sf::Vector2f(1, tileSize - 14));
	hitbox.setPoint(2, sf::Vector2f(tileSize -1, tileSize - 14));
	hitbox.setPoint(3, sf::Vector2f(tileSize - 1, tileSize));
	tilesHitboxes.push_back(hitbox);

	//6 barrel
	hitbox.setPointCount(4);
	hitbox.setPoint(0, sf::Vector2f(3, tileSize));
	hitbox.setPoint(1, sf::Vector2f(3, 0));
	hitbox.setPoint(2, sf::Vector2f(tileSize - 3, 0));
	hitbox.setPoint(3, sf::Vector2f(tileSize - 3, tileSize));
	tilesHitboxes.push_back(hitbox);

	//7 door
	hitbox.setPointCount(4);
	hitbox.setPoint(0, sf::Vector2f(2, tileSize));
	hitbox.setPoint(1, sf::Vector2f(2, 0));
	hitbox.setPoint(2, sf::Vector2f(tileSize - 26, 0));
	hitbox.setPoint(3, sf::Vector2f(tileSize - 26, tileSize));
	tilesHitboxes.push_back(hitbox);
}

bool TileMap::pointInPolygon(int x, int y, int polygonId) {
	int count = 0, countParallel = 0;
	sf::ConvexShape* hitboxPtr = &tilesHitboxes[polygonId];
	
	double x1 = x % tileSize;
	double y1 = y % tileSize;
	
	double x2 = tileSize;
	double y2 = y1;
	double x3, y3, x4, y4;

	for (size_t i = 0; i < hitboxPtr->getPointCount(); i++) {

		x3 = hitboxPtr->getPoint(i).x;
		y3 = hitboxPtr->getPoint(i).y;
		x4 = hitboxPtr->getPoint((i + 1) % hitboxPtr->getPointCount()).x;
		y4 = hitboxPtr->getPoint((i + 1) % hitboxPtr->getPointCount()).y;
		


		double Ua, Ub, numerator_a, numerator_b, denominator;
		denominator = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);
		if (denominator == 0) {

			if (/*((x1 > x3 && x1 < x4) || (x1 < x3 && x1 > x4))&&*/y1 == y3 && x1 <= x4) {//(x1 * y2 - x2 * y1) * (x4 - x3) - (x3 * y4 - x4 * y3) * (x2 - x1) == 0 && (x1 * y2 - x2 * y1) * (y4 - y3) - (x3 * y4 - x4 * y3) * (y2 - y1) == 0) {
				
				
				if (x1<x3 || x1>x4) {
					return false;
				}
			//	count++;

			}
			if ((x1>=x3 && x1<=x4)&& y1 == y3) {
		//		std::cout << y1 << std::endl;
				return true;
			}


		}
		else {
			numerator_a = 1.0*(x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
			numerator_b = 1.0*(x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
			Ua = numerator_a / denominator;
			Ub = numerator_b / denominator;
			if (Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1) {
				//std::cout << "numerator" << std::endl;
				count++;
			}
		}
	}
	//std::cout << countParallel << std::endl;


	
	if (count % 2 == 0) {
		return false;
	}
	return true;

}

void TileMap::computeOffsets(int x, int y, int polygonId) {
	sf::ConvexShape* hitboxPtr = &tilesHitboxes[polygonId];

	for (size_t i = 0; i < hitboxPtr->getPointCount()-1; i += 1) {
		if ((hitboxPtr->getPoint(i).x <= x && hitboxPtr->getPoint(i + 1).x >= x) || (hitboxPtr->getPoint(i).x >= x && hitboxPtr->getPoint(i + 1).x <= x)) {
			if (hitboxPtr->getPoint(i).y <= y) {
				bottomSideDistance = hitboxPtr->getPoint(i).y;
			}
		}
		if ((hitboxPtr->getPoint(i).y <= y && hitboxPtr->getPoint(i + 1).y >= y) || (hitboxPtr->getPoint(i).y >= y && hitboxPtr->getPoint(i + 1).y <= y)) {
			if (hitboxPtr->getPoint(i).x <= x) {
				rightSideDistance = hitboxPtr->getPoint(i).x;
			}
		}
		if ((hitboxPtr->getPoint(i).y <= y && hitboxPtr->getPoint(i + 1).y >= y) || (hitboxPtr->getPoint(i).y >= y && hitboxPtr->getPoint(i + 1).y <= y)) {
			if (hitboxPtr->getPoint(i).x >= x) {
				leftSideDistance = hitboxPtr->getPoint(i).x;

			}
		}
	}



}

