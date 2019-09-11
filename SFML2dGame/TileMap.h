#pragma once
#include "DEFINITIONS.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>

#include "Game.h"

class TileMap
{
public:
	TileMap(GameDataRef data, int visibleWidth, int visibleHeight);
	TileMap(GameDataRef data, int width, int height, bool editMode);
	void draw();
	void drawEditMap();
	void canvasUpdate(int x);
	void placeObject(int x, int y, int id);
	void changeTile(int x, int y, int id, bool blockTiles);
	bool checkCollisionOfPoint(int x, int y);
	int getDamageFromObject();
	void endEditingMap(std::string filename);
	int bottomSideDistance, rightSideDistance, leftSideDistance, mapWidth, mapHeight;
	void backgroundUpdate(float dt, int dx, int x);
	sf::Sprite *mapSpritePtr = &mapSprite, *backgroundPtr = &background;
	std::list<std::tuple<int, int , sf::IntRect>> triggers;
private:
	int damageFromObject = 0;
	bool _editMode;
	int editMapWidth, editMapHeight;
	GameDataRef _data;
	void initMap();
	void newMap();
	std::vector<std::map<std::string, sf::VertexArray>> map;
	std::deque<std::map<std::string, sf::VertexArray>*> mapObserve;
	

	std::vector<std::vector<sf::Vector2<short int>>> idData;

	void readValue(std::string &line, std::string &value);

	void setTilePosition(sf::Vertex* vertexPtr, int x, int y);
	void setTileTextureCords(sf::Vertex* vertexPtr, int x, int y);
	

	sf::Sprite mapSprite;
	sf::RenderTexture canvas;

	

	sf::Vector2i currentBlock;
	

	int tilesInRow = windowWidth/tileSize, tilesInCol = windowHeight/tileSize;

	int tileTextureWidth = _data->asset.getTexture("Tiles Texture").getSize().x, tileTextureHeight = _data->asset.getTexture("Tiles Texture").getSize().y;
	int objectTextureWidth = _data->asset.getTexture("Objects Texture").getSize().x, objectTextureHeight = _data->asset.getTexture("Objects Texture").getSize().y;

	void reDrawCanvas();
	sf::Sprite background;
	sf::Sprite background1;
	sf::Sprite background2;
	sf::RenderTexture backgroundCanvas;
	

	void initMapBlocks(std::map<std::string, sf::VertexArray> &block);

	sf::Sprite visibleRect;
	int _visibleWidth, _visibleHeight;

	void initTilesHitboxes();
	std::vector<sf::ConvexShape> tilesHitboxes;
	sf::Vector2<short int> idBuf;
	bool pointInPolygon(int x, int y, int polygonId);
	void computeOffsets(int x, int y, int polygonId);
	sf::Clock clock;
	std::list<sf::Sprite> clouds;

	
	void addTrigger(int x, int y, short int objectId);
};

