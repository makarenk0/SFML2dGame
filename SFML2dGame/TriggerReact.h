#pragma once
#include "TileMap.h"
#include <tuple>

class TriggerReact {
public:
	TriggerReact(TileMap* map, std::tuple<int , int, sf::IntRect> trigger, int frameTime);
	void update();
	bool _remove = false;
private:
	TileMap* _map;
	std::tuple<int,int, sf::IntRect> _trigger;
	sf::Clock* triggerLife;
	int _frameTime;
	int _counter=1;
	int objTextureWidth = 17;
	
};