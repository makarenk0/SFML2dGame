#pragma once
#include "State.h"
#include "Game.h"
#include "TileMap.h"
#include "DEFINITIONS.h"
#include <iostream>
#include "TileMapEditor.h"
#include "EnterMapNameState.h"
#include "GameState.h"
#include "QuickMenuBar.h"

class MapEditorState : public State
{
public:
	MapEditorState(GameDataRef data, std::string customMapName);
	MapEditorState(GameDataRef data, int mapWidth, int mapHeight);
	~MapEditorState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

	void Pause();
	void Resume();

private:
	int _mapWidth, _mapHeight;
	bool _toTest;
	std::string _customMapName = "";


	sf::View view;
	
	int mouseX, mouseY;

	
	int inputSizeState = 0;
	TileMapEditor* map;
	GameDataRef _data;
	sf::Sprite _background;

	QuickMenuBar *_quickMenuBar;

	sf::Sprite toolWindow;
	sf::Sprite _selectIndicator;
	sf::RenderTexture toolCanvas;
	int toolWidth;
	int offset;
	bool visibleTools = true;

	void scaleView(int deltaScale);
	void scrollView(sf::Vector2i deltaScroll);

	int _toolWindowScroll = 0;
	int _scrollSpeedCoef = 10;
	void scrollToolWindow(int deltaScale);

	void redrawToolWindow();

	void toolWindowUpdate();

	void goToTesting();

	void resetCamera();

	sf::Sprite arrowButton;

	std::vector<sf::Sprite> toolSprites;
	int id = 0;

	void initToolSprites();
	
};

