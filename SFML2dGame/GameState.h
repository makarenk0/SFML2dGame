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
	GameState(GameDataRef data);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	int dir, speed = 0;
	TileMap* map;
	Player* player;
	GameDataRef _data;
	sf::Sprite _background;
	bool finishState;
};

