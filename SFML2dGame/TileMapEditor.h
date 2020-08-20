#pragma once
#include "BaseMap.h"
#include <iostream>
#include <fstream>

class TileMapEditor : public BaseMap
{
public:
	TileMapEditor(GameDataRef data, int width, int height);
	TileMapEditor(GameDataRef data, const std::string& _customMapName);
	void placeObject(int x, int y, int id);
	void endEditingMap(std::string filename);
private:
	bool _editMode;
	int _editMapWidth, _editMapHeight;
	std::vector<std::vector<sf::Vector2<short int>>> idData;

	int playerStartPosX = -1, playerStartPosY = -1;
	int portalX = -1, portalY = -1;

	void reDrawCanvas();
	void newMap();
	void initMap(std::string mapName, bool campaignMode);
	void setTilePosition(sf::Vertex* vertexPtr, int x, int y);
};

