#pragma once
#include "DEFINITIONS.h"
#include <deque>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include "BaseMap.h"
#include "TriggerReact.h"
#include "Inventory.h"
#include "CoinsParticlesSet.h"

class TileMap : public BaseMap
{
public:
	TileMap(GameDataRef data, int visibleWidth, int visibleHeight, std::string mapName, bool campaignMode);
	bool checkCollisionOfPoint(int x, int y);
	int getDamageFromObject();
	int bottomSideDistance, rightSideDistance, leftSideDistance, mapWidth, mapHeight;
	void mapAnimationsUpdate(float dt, int dx, int x);
	sf::Sprite *mapSpritePtr = &mapSprite, *backgroundPtr = &background;
	//std::vector<std::tuple<int, int , sf::IntRect>> triggers;
	Inventory* _playerInventory;
	int playerStartPosX = -1, playerStartPosY = -1;
	int portalX = -1, portalY = -1;
	bool playerWin = false;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	int damageFromObject = 0;
	
	void initMap(std::string mapName, bool campaignMode);
	std::vector<std::vector<sf::Vector2<short int>>> idData;

	std::vector<TriggerReact*> _triggers;

	bool checkIfSameTrigger(int x, int y);

	void setTilePosition(sf::Vertex* vertexPtr, int x, int y);
	
	sf::Sprite background;
	sf::Sprite background1;
	sf::Sprite background2;
	sf::RenderTexture backgroundCanvas;
	
	void reDrawCanvas();

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

