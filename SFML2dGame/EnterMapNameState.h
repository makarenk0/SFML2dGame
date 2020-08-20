#pragma once
#include "State.h"
#include "Game.h"
#include "TileMapEditor.h"

class EnterMapNameState : public State
{
public:
	EnterMapNameState(GameDataRef data, TileMapEditor* map);
	EnterMapNameState(GameDataRef data, TileMapEditor* map, std::string* customMapName);
	~EnterMapNameState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
private:
	TileMapEditor* _map;
	TileMapEditor* _backgroundMap;
	sf::Clock triggerLife;
	sf::Text _mapNameDisplay;
	std::string _mapName = "";
	std::string* _mapNamePtr;
	GameDataRef _data;
};

