#pragma once
#include "State.h"
#include "Game.h"
#include "TileMap.h"
#include "Player.h"
#include "DEFINITIONS.h"
#include "ParticlesSystem.h"
#include "BloodParticlesSet.h"
#include <iostream>
#include <thread>

class GameState : public State
{
public:
	GameState(GameDataRef data, std::string mapName, bool campaignMode);
	~GameState();

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
	std::thread _parallelParticles;
	bool finishState = false;
};

