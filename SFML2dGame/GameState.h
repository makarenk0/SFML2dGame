#pragma once
#include "State.h"
#include "Game.h"
#include "TileMap.h"
#include "Player.h"
#include "DEFINITIONS.h"
#include <iostream>

class GameState : public State
{
public:
	GameState(GameDataRef data, std::string mapName, bool campaignMode);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	bool _campaignMode;
	std::string _mapName;
	int dir = 0, speed = 0;
	TileMap* map = nullptr;
	Player* player = nullptr;
	GameDataRef _data;
	sf::Sprite _background;
	bool finishState = false;
};

