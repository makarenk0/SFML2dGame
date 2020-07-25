#pragma once
#include "DEFINITIONS.h"
#include <deque>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include "BaseMap.h"

class TileMap : public BaseMap
{
public:
	TileMap(GameDataRef data, int visibleWidth, int visibleHeight, std::string mapName, bool campaignMode);
	void canvasUpdate(int x);
	bool checkCollisionOfPoint(int x, int y);
	int getDamageFromObject();
	int bottomSideDistance, rightSideDistance, leftSideDistance, mapWidth, mapHeight;
	void mapAnimationsUpdate(float dt, int dx, int x);
	sf::Sprite *mapSpritePtr = &mapSprite, *backgroundPtr = &background;
	std::list<std::tuple<int, int , sf::IntRect>> triggers;
	int playerStartPosX = -1, playerStartPosY = -1;
	int portalX = -1, portalY = -1;
	bool playerWin = false;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	int damageFromObject = 0;
	
	void initMap(std::string mapName, bool campaignMode);
	std::deque<std::map<std::string, sf::VertexArray>*> mapObserve;
	

	std::vector<std::vector<sf::Vector2<short int>>> idData;

	void readValue(std::string &line, std::string &value);

	void setTilePosition(sf::Vertex* vertexPtr, int x, int y);
	void setTileTextureCords(sf::Vertex* vertexPtr, int x, int y);

	sf::Vector2i currentBlock;
	

	

	

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

