#include "TileMapEditor.h"

TileMapEditor::TileMapEditor(GameDataRef data, int width, int height) : BaseMap(data)
{
	_editMapWidth = width;
	_editMapHeight = height;
	idData.resize(_editMapHeight / tileSize);
	//	initMap();  // add editing created maps(for future);
	newMap();
}

void TileMapEditor::setTilePosition(sf::Vertex* vertexPtr, int x, int y) {
	vertexPtr[0].position = sf::Vector2f(x * tileSize, y * tileSize);
	vertexPtr[1].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize);
	vertexPtr[2].position = sf::Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
	vertexPtr[3].position = sf::Vector2f(x * tileSize, y * tileSize + tileSize);
}



void TileMapEditor::newMap() {
	canvas.create(_editMapWidth, _editMapHeight);
	canvas.display();
	_tilesInCol = _editMapHeight / tileSize;

	for (int i = 0; i < _editMapWidth / windowWidth; i++) {
		std::map<std::string, sf::VertexArray> block;
		initMapBlocks(block);

		for (int y = 0; y < _tilesInCol; y++) {
			for (int x = 0; x < _tilesInRow; x++) {
				sf::Vertex* vertexTile = &block["blockTiles"][4.0 * (1.0 * y * _tilesInRow + x)];
				setTilePosition(vertexTile, x + i * _tilesInRow, y);
				setTileTextureCords(vertexTile, 0, 0);

				sf::Vertex* vertexObject = &block["blockObjects"][4.0 * (1.0 * y * _tilesInRow + x)];
				setTilePosition(vertexObject, x + i * _tilesInRow, y);
				setTileTextureCords(vertexObject, 0, 0);
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

void TileMapEditor::canvasUpdate(int x) {  //for rendering only part of tilemap
	if (currentBlock.y != (int)(x / windowWidth)) {
		currentBlock.y = x / windowWidth;
		if (currentBlock.x == 1) {
			currentBlock.x = 0;
		}
		else {
			currentBlock.x = 1;
		}
	}


	if (x % windowWidth < windowWidth / 2 && x > windowWidth) {
		if (currentBlock.x == 0) {
			mapObserve.push_front(&map.at(currentBlock.y - 1.0));
			mapObserve.pop_back();
			currentBlock.x = 1;
			reDrawCanvas();
		}

	}
	else if (x % windowWidth > windowWidth / 2 && x < canvas.getSize().x - windowWidth) {
		if (currentBlock.x == 1) {
			mapObserve.push_back(&map.at(currentBlock.y + 1.0));
			mapObserve.pop_front();
			currentBlock.x = 0;
			reDrawCanvas();
		}

	}

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
		}

	}

}

void TileMapEditor::endEditingMap(std::string filename) {
	std::ofstream customMap;
	customMap.open("maps/custom/" + filename + ".txt");
	customMap << "player1(" << playerStartPosX << "," << playerStartPosY << ",)" << std::endl;
	changeTile(playerStartPosX, playerStartPosY, 0, false);
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

void TileMapEditor::initMapBlocks(std::map<std::string, sf::VertexArray>& block) {
	block["blockTiles"] = sf::VertexArray();
	block["blockObjects"] = sf::VertexArray();
	block["blockTiles"].resize(4.0 * _tilesInRow * _tilesInCol);
	block["blockObjects"].resize(4.0 * _tilesInRow * _tilesInCol);
	block["blockTiles"].setPrimitiveType(sf::Quads);
	block["blockObjects"].setPrimitiveType(sf::Quads);
}

