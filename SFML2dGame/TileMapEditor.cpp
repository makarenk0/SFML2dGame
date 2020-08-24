#include "TileMapEditor.h"

TileMapEditor::TileMapEditor(GameDataRef data, int width, int height) : BaseMap(data)
{
	_editMapWidth = width;
	_editMapHeight = height;
	idData.resize(_editMapHeight / tileSize);
	newMap();
}

TileMapEditor::TileMapEditor(GameDataRef data, const std::string& _customMapName) : BaseMap(data) {
	initMap(_customMapName, false);
}

void TileMapEditor::setTilePosition(sf::Vertex* vertexPtr, int x, int y) {
	vertexPtr[0].position = sf::Vector2f(x * tileSize, y * tileSize);
	vertexPtr[1].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize);
	vertexPtr[2].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
	vertexPtr[3].position = sf::Vector2f(x * tileSize, y * tileSize + tileSize);
}



void TileMapEditor::reDrawCanvas()
{
	canvas.clear(sf::Color(30, 144, 255));
	BaseMap::reDrawCanvas();
}

void TileMapEditor::newMap() {
	canvas.create(_editMapWidth, _editMapHeight);
	canvas.display();
	_tilesInRow = _editMapWidth / tileSize;
	_tilesInCol = _editMapHeight / tileSize;

	for (int i = 0; i < _tilesInRow / MAP_BLOCK_WIDTH; i++) {
		std::map<std::string, sf::VertexArray> block;
		initMapBlocks(block);

		for (int y = 0; y < _tilesInCol; y++) {
			for (int x = 0; x < MAP_BLOCK_WIDTH; x++) {
				sf::Vertex* vertexTile = &block["blockTiles"][4.0 * (1.0 * y * MAP_BLOCK_WIDTH + x)];
				setTilePosition(vertexTile, x + i * MAP_BLOCK_WIDTH, y);
				setTileTextureCords(vertexTile, 0, 0);

				sf::Vertex* vertexObject = &block["blockObjects"][4.0 * (1.0 * y * MAP_BLOCK_WIDTH + x)];
				setTilePosition(vertexObject, x + i * MAP_BLOCK_WIDTH, y);
				setTileTextureCords(vertexObject, 0, 0);
			}
		}
		map.push_back(block);
	}
	reDrawCanvas();
}


void TileMapEditor::initMap(std::string mapName, bool campaignMode) {
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
	_editMapWidth = std::stoi(value);
	readValue(line, value);
	_editMapHeight = std::stoi(value);
	_tilesInCol = _editMapHeight / tileSize;
	_tilesInRow = _editMapWidth / tileSize;

	canvas.create(_editMapWidth, _editMapHeight);
	idData.resize(_tilesInCol);
	canvas.clear();
	canvas.display();

	std::map<std::string, sf::VertexArray> block;
	initMapBlocks(block);


	for (int i = 0;; i++) {
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
		setTileTextureCords(vertexTile, tileId % (_tileTextureWidth / tileSize), tileId / (_tileTextureWidth / tileSize));

		sf::Vertex* vertexObject = &block["blockObjects"][i * 4.0];
		setTilePosition(vertexObject, x, y);
		setTileTextureCords(vertexObject, objectId % (_tileTextureWidth / tileSize), objectId / (_tileTextureWidth / tileSize));

	}
	file.close();
	//test
	changeTile(playerStartPosX, playerStartPosY, 26, false);
	reDrawCanvas();
}

void TileMapEditor::placeObject(int x, int y, int id) {

	if (id >= 0 && id < (_tileTextureWidth / tileSize) * (_tileTextureHeight / tileSize)) {
		if (id == 0) {
			changeTile(x, y, id, true);
		}
		else {
			changeTile(x, y, id, true);
		}
	}
	else {
		id = id % ((_tileTextureWidth / tileSize) * (_tileTextureHeight / tileSize));
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
		case 17:
			if (playerStartPosX != -1) {
				changeTile(playerStartPosX, playerStartPosY, 0, false);
			}
			changeTile(x, y, 26, false);
			playerStartPosX = x;
			playerStartPosY = y;

			break;
		case 18:
			if (portalX != -1) {
				changeTile(portalX, portalY, 0, false);
			}
			changeTile(x, y, 27, false);
			portalX = x;
			portalY = y;
			break;
		case 19:
			changeTile(x, y, 28, false);
			break;
		case 20:
			changeTile(x, y, 29, false);
			break;
		case 21:
			changeTile(x, y, 30, false);
			break;
		case 22:
			changeTile(x, y, 31, false);
			break;
		case 23:
			changeTile(x, y, 32, false);
			break;
		case 24:
			changeTile(x, y, 37, false);
			break;
		case 25:
			changeTile(x, y, 38, false);
			break;
		case 26:
			changeTile(x, y, 39, false);
			break;
		case 27:
			changeTile(x, y, 40, false);
			break;
		case 28:
			changeTile(x, y, 41, false);
			break;
		case 29:
			changeTile(x, y, 42, false);
			break;
		case 30:
			changeTile(x, y, 45, false);
			break;
		case 31:
			changeTile(x, y, 46, false);
			break;
		case 32:
			changeTile(x, y, 47, false);
			break;
		case 33:
			changeTile(x, y, 48, false);
			break;
		case 34:
			changeTile(x, y, 49, false);
			break;
		case 35:
			changeTile(x, y, 50, false);
			break;
		case 36:
			changeTile(x, y, 69, false);
			break;
		case 37:
			changeTile(x, y, 70, false);
			break;
		case 38:
			changeTile(x, y, 77, false);
			break;
		case 39:
			changeTile(x, y, 78, false);
			break;
		}

	}

}

void TileMapEditor::endEditingMap(std::string filename) {
	std::ofstream customMap;
	customMap.open("maps/custom/" + filename + ".txt", std::ofstream::trunc);
	customMap << "player1(" << playerStartPosX << "," << playerStartPosY << ",)" << std::endl;
	//changeTile(playerStartPosX, playerStartPosY, 0, false);
	customMap << "mapBound(" << canvas.getSize().x << "," << canvas.getSize().y << ",)" << std::endl;

	int blockNum = 0;
	for (auto& i : map) {
		for (int j = 0; j < i["blockTiles"].getVertexCount(); j += 4) {
			customMap << "(" << i["blockTiles"][j].position.x / tileSize << "," << i["blockTiles"][j].position.y / tileSize << ","
				<< i["blockTiles"][j].texCoords.x / tileSize + (i["blockTiles"][j].texCoords.y / tileSize * _tileTextureWidth / tileSize) << ","
				<< i["blockObjects"][j].texCoords.x / tileSize + (i["blockObjects"][j].texCoords.y / tileSize * _tileTextureWidth / tileSize) << ","
				<< 0 << ",)" << std::endl;
		}
		blockNum++;
		customMap << "endBlock" << std::endl;

	}
	customMap << "end" << std::endl;
	customMap.close();
}

