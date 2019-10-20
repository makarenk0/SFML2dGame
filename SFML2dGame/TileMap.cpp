#include "TileMap.h"



TileMap::TileMap(GameDataRef data, int visibleWidth, int visibleHeight) : _data(data)
{
	_visibleWidth = visibleWidth;
	_visibleHeight = visibleHeight;
	
	mapSprite.setPosition(0, 0);
	initTilesHitboxes();
	srand(time(0));


	initMap();
}

TileMap::TileMap(GameDataRef data, int width, int height, bool editMode) : _data(data), _editMode(editMode)
{
	editMapWidth = width;
	editMapHeight = height;
	mapSprite.setPosition(0, 0);
	idData.resize(editMapHeight / tileSize);
//	initMap();
	newMap();
}

void TileMap::initMap() {
	std::ifstream file("maps/secondMap.txt");
	std::string line;
	std::string value = "";
	int x, y, tileId, objectId, blockNum = 0;
	bool solid;

	//init canvas that fits tilemap
	std::getline(file, line);
	line = line.substr(8, line.length());
	readValue(line, value);
	mapWidth = std::stoi(value);
	readValue(line, value);
	mapHeight = std::stoi(value);
	tilesInCol = mapHeight / tileSize;

	canvas.create(mapWidth, mapHeight);
	idData.resize(tilesInCol);
	canvas.clear();
	canvas.display();

	//--------------------------------
	backgroundCanvas.create(_visibleWidth, tilesInCol*tileSize);
	background1.setTexture(_data->asset.getTexture("Background1"));
	background1.setPosition(0, tilesInCol * tileSize / 2);
	background1.setScale(1, tilesInCol * tileSize/2/ _data->asset.getTexture("Background1").getSize().y);

	background2.setTexture(_data->asset.getTexture("Background2"));
	background2.setPosition(0, 0);
	background2.setScale(1, tilesInCol * tileSize / 2 / _data->asset.getTexture("Background2").getSize().y);
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

		idData[i / tilesInRow].push_back(sf::Vector2<short int>(tileId, objectId));

		addTrigger(x, y, objectId);
		//-----------------------
		sf::Vertex* vertexTile = &block["blockTiles"][i * 4.0];
		setTilePosition(vertexTile, x, y);
		setTileTextureCords(vertexTile, tileId%(tileTextureWidth/tileSize), tileId/(tileTextureWidth /tileSize));

		sf::Vertex* vertexObject = &block["blockObjects"][i * 4.0];
		setTilePosition(vertexObject, x, y);
		setTileTextureCords(vertexObject, objectId % (tileTextureWidth / tileSize), objectId / (tileTextureWidth / tileSize));
		
	}
	file.close();
	mapObserve.push_back(&map.at(0));
	mapObserve.push_back(&map.at(1));
	currentBlock.y = 0;
	currentBlock.x = 0;
	//test
	reDrawCanvas();
}

void TileMap::readValue(std::string &line, std::string &value) {
	line = line.substr(value.length() + 1, line.length());
	value = line.substr(0, line.find(","));
}

void TileMap::addTrigger(int x, int y, short int objectId) {
	if (objectId > 0&& objectId < 3) {
		triggers.push_back(std::tuple<int, int, sf::IntRect>(objectId, 4 ,sf::IntRect(x*tileSize , y*tileSize, tileSize, tileSize)));
	}
	else if (objectId == 16) {
		triggers.push_back(std::tuple<int, int, sf::IntRect>(objectId, 2, sf::IntRect(x * tileSize, y * tileSize, tileSize, tileSize)));
	}
	
}

void TileMap::setTilePosition(sf::Vertex* vertexPtr, int x, int y) {
	vertexPtr[0].position = sf::Vector2f(x * tileSize, y * tileSize);
	vertexPtr[1].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize);
	vertexPtr[2].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
	vertexPtr[3].position = sf::Vector2f(x * tileSize, y * tileSize + tileSize);
}

void TileMap::setTileTextureCords(sf::Vertex* vertexPtr, int x, int y) {
	vertexPtr[0].texCoords = sf::Vector2f(x * tileSize, y * tileSize);
	vertexPtr[1].texCoords = sf::Vector2f(x * tileSize + tileSize, y * tileSize);
	vertexPtr[2].texCoords = sf::Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
	vertexPtr[3].texCoords = sf::Vector2f(x * tileSize, y * tileSize + tileSize);
}

void TileMap::draw() {
	_data->window.draw(background);
	_data->window.draw(mapSprite);
}

void TileMap::backgroundUpdate(float dt, int dx, int x) {
	backgroundCanvas.clear();
	backgroundCanvas.draw(background2);
	backgroundCanvas.draw(background1);
	if (clock.getElapsedTime().asSeconds() > 5) {
		sf::Sprite cloud;
		cloud.setTexture(_data->asset.getTexture("Clouds"));
		cloud.setTextureRect(sf::IntRect(rand() % 4 * 166, 0, 166, 53));
		cloud.setPosition(_visibleWidth, rand() % tilesInCol*tileSize / 3);
		clouds.push_back(cloud);
		clock.restart();
	}
		for (auto& i : clouds) {
			i.move(-CLOUDS_SPEED - dt * dx, 0.f);
			backgroundCanvas.draw(i);
		}
		if (!clouds.empty()&&clouds.front().getPosition().x + 166 < 0) {
			clouds.pop_front();
		}
	background.setTexture(backgroundCanvas.getTexture());
}

void TileMap::drawEditMap() {

	_data->window.draw(mapSprite);
}

void TileMap::canvasUpdate(int x) {  //for rendering only part of tilemap
	if (currentBlock.y != (int)(x / windowWidth)) {
		currentBlock.y = x / windowWidth;
		if (currentBlock.x == 1){
			currentBlock.x = 0;
		}
		else {
			currentBlock.x = 1;
		}
	}
	

	if (x % windowWidth < windowWidth / 2&& x > windowWidth) {
		if (currentBlock.x == 0) {
			mapObserve.push_front(&map.at(currentBlock.y - 1));
			mapObserve.pop_back();
			currentBlock.x = 1;
			reDrawCanvas();
		}
		
	}
	else if (x % windowWidth > windowWidth / 2 && x < canvas.getSize().x - windowWidth) {
		if (currentBlock.x == 1) {
			mapObserve.push_back(&map.at(currentBlock.y + 1));
			mapObserve.pop_front();
			currentBlock.x = 0;
			reDrawCanvas();
		}
		
	}

	
	
}

void TileMap::newMap() {
	canvas.create(editMapWidth, editMapHeight);
	canvas.display();
	tilesInCol = editMapHeight / tileSize;
	
	for (int i = 0; i < editMapWidth / windowWidth; i++) {
		std::map<std::string, sf::VertexArray> block;
		initMapBlocks(block);

		for (int y = 0; y < tilesInCol; y++) {
			for (int x = 0; x < tilesInRow; x++) {
				sf::Vertex* vertexTile = &block["blockTiles"][4*(y * tilesInRow + x)];
				setTilePosition(vertexTile, x + i*tilesInRow, y);
				setTileTextureCords(vertexTile, 0, 0);

				sf::Vertex* vertexObject = &block["blockObjects"][4 * (y * tilesInRow + x)];
				setTilePosition(vertexObject, x + i * tilesInRow, y);
				setTileTextureCords(vertexObject, 0, 0);

				//solidData[y].push_back(false);
			}
		}
		map.push_back(block);
	}
	mapObserve.push_back(&map[0]);
	mapObserve.push_back(&map[1]);
	currentBlock.y = 0;
	currentBlock.x = 0;
	reDrawCanvas();
}

void TileMap::placeObject(int x, int y, int id) {
	
	if (id >= 0 && id < (tileTextureWidth/tileSize)* (tileTextureHeight / tileSize)) {
		if (id == 0) {
			changeTile(x, y, id, true);
		}
		else {
			changeTile(x, y, id, true);
		}
	}
	else {
		id = id%((tileTextureWidth / tileSize) * (tileTextureHeight / tileSize));
		
		switch (id) {
		case 0:
			changeTile(x, y, 0, false);
			break;
		case 1:
			changeTile(x, y, 1, false);
			break;
		case 2:
			changeTile(x, y, 2, false);
			break;
		case 3:
			changeTile(x, y, id + 6, false);
			changeTile(x - tileSize, y - tileSize, id + 3, false);
			changeTile(x, y - tileSize, id + 4, false);
			changeTile(x + tileSize, y - tileSize, id + 5, false);
			changeTile(x - tileSize, y - 2 * tileSize, id, false);
			changeTile(x, y - 2 * tileSize, id + 1, false);
			changeTile(x + tileSize, y - 2 * tileSize, id + 2, false);

			break;
		case 4:
			changeTile(x, y, 10, false);
			break;
		case 5:
			changeTile(x, y, 11, false);
			break;
		case 6:
			changeTile(x, y, 12, false);
			break;
		case 7:
			changeTile(x, y, 13, false);
			break;
		case 8:
			changeTile(x, y, 14, false);
			break;
		case 9:
			changeTile(x, y, 15, false);
			break;
		case 10:
			changeTile(x, y, 16, false);
			break;
		case 11:
			changeTile(x, y, 20, false);
			break;
		case 12:
			changeTile(x, y, 21, false);
			break;
		case 13:
			changeTile(x, y, 22, false);
			break;
		case 14:
			changeTile(x, y, 23, false);
			break;
		case 15:
			changeTile(x, y, 24, false);
			break;
		case 16:
			changeTile(x, y, 25, false);
			break;
		}

	}
	
}

void TileMap::changeTile(int x, int y, int id, bool blockTiles) {
	
	if (x > 0 && x < canvas.getSize().x && y>0 && y < canvas.getSize().y) {
		int blockNum = x / windowWidth;
	//	solidData[y / tileSize][x / tileSize] = solid;
	//	idData[y / tileSize][x / tileSize] = id;
		
		if (blockTiles) {
			sf::Vertex* vertexTile = &map[blockNum]["blockTiles"][4 * (((y / tileSize) * tilesInRow) + ((x % windowWidth) / tileSize))];
			setTileTextureCords(vertexTile, id % (tileTextureWidth / tileSize), id / (tileTextureWidth / tileSize));	
		}
		else {
			sf::Vertex* vertexObject = &map[blockNum]["blockObjects"][4 * (((y / tileSize) * tilesInRow) + ((x % windowWidth) / tileSize))];
			setTileTextureCords(vertexObject, id % (objectTextureWidth / tileSize), id / (objectTextureWidth / tileSize));
		}
	
		reDrawCanvas();
}
}

void TileMap::endEditingMap(std::string filename) {
	std::ofstream customMap;
	customMap.open("maps/" + filename+".txt");
	customMap << "mapBound(" << canvas.getSize().x << "," << canvas.getSize().y << ",)" << std::endl;

	int blockNum = 0;
	for (auto& i : map) {
		for (int j = 0; j < i["blockTiles"].getVertexCount(); j += 4) {
			customMap << "(" << i["blockTiles"][j].position.x/tileSize << "," << i["blockTiles"][j].position.y/tileSize << "," 
				<< i["blockTiles"][j].texCoords.x / tileSize+ (i["blockTiles"][j].texCoords.y/tileSize*tileTextureWidth/tileSize) <<","
				<< i["blockObjects"][j].texCoords.x / tileSize + (i["blockObjects"][j].texCoords.y / tileSize * tileTextureWidth / tileSize)<<","
				<<0<< ",)" << std::endl;
		}
		blockNum++;
		customMap << "endBlock" << std::endl;

	}
	customMap << "end" << std::endl;
	customMap.close();
}

void TileMap::reDrawCanvas() {
	if (_editMode) {
		canvas.clear(sf::Color(30, 144, 255));
	}
	else {
		canvas.clear(sf::Color::Transparent);
	}
	

	for (auto &i : mapObserve) {
		canvas.draw(i->at("blockTiles"), &_data->asset.getTexture("Tiles Texture"));
		canvas.draw(i->at("blockObjects"), &_data->asset.getTexture("Objects Texture"));
	}
	mapSprite.setTexture(canvas.getTexture());
}

bool TileMap::checkCollisionOfPoint(int x, int y) {
	damageFromObject = 0;
	bottomSideDistance = 0;
	rightSideDistance = 0;
	leftSideDistance = 32;

	idBuf = idData[y / tileSize][x / tileSize];
	if (idBuf.y == 0) {
		int id = idBuf.x;
		if (id == 0||id>32) {
			return false;
		}
		if (id > 0 && id < 20) {
			bottomSideDistance = 0;
			leftSideDistance = 32;
			rightSideDistance = 0;
			return true;
		}
		else if (id > 19 && id < 27) {
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
	}
	else {
		int id = idBuf.y;
		
		if (id > 0 && id < 3) {
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
		else if ((id > 2&& id<18)||id==24) {
			

			return false;
		}
		
		
	}

}

int TileMap::getDamageFromObject() {
	return damageFromObject;
}

void TileMap::initMapBlocks(std::map<std::string, sf::VertexArray> &block) {
	block["blockTiles"] = sf::VertexArray();
	block["blockObjects"] = sf::VertexArray();
	block["blockTiles"].resize(4 * tilesInRow * tilesInCol);
	block["blockObjects"].resize(4 * tilesInRow * tilesInCol);
	block["blockTiles"].setPrimitiveType(sf::Quads);
	block["blockObjects"].setPrimitiveType(sf::Quads);
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
}

bool TileMap::pointInPolygon(int x, int y, int polygonId) {
	int count = 0, countParallel = 0;
	sf::ConvexShape* hitboxPtr = &tilesHitboxes[polygonId];
	
	int x1 = x % tileSize;
	int y1 = y % tileSize;
	
	int x2 = tileSize;
	int y2 = y1;
	int x3, y3, x4, y4;

	for (int i = 0; i < hitboxPtr->getPointCount(); i++) {

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
			numerator_a = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
			numerator_b = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
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
	//std::vector<int> yPoint;


	

	for (int i = 0; i < hitboxPtr->getPointCount()-1; i += 1) {
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