#pragma once
#include "BaseMap.h"
#include <iostream>
#include <fstream>

class TileMapEditor : public BaseMap
{
public:
	TileMapEditor(GameDataRef data, int width, int height);
	void placeObject(int x, int y, int id);
	void endEditingMap(std::string filename);
	void canvasUpdate(int x);
private:
	bool _editMode;
	int _editMapWidth, _editMapHeight;
	std::vector<std::vector<sf::Vector2<short int>>> idData;
	
	sf::Vector2i currentBlock;

	int playerStartPosX = -1, playerStartPosY = -1;
	int portalX = -1, portalY = -1;

	void newMap();
	void setTilePosition(sf::Vertex* vertexPtr, int x, int y);
	void initMapBlocks(std::map<std::string, sf::VertexArray>& block);
};

