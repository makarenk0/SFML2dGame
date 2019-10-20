#pragma once
#include "State.h"
#include "Game.h"
#include "TileMap.h"
#include "DEFINITIONS.h"
#include <iostream>

class MapEditorState : public State
{
public:
	MapEditorState(GameDataRef data);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	sf::View view;
	
	int mouseX, mouseY;

	sf::Text inputWidth, inputHeight, explanationText, inputName;
	std::string inputWidthText, inputHeightText, mapName="";
	int mapWidthInput=1600, mapHeightInput=800;
	int inputSizeState = 0;
	TileMap* map;
	GameDataRef _data;
	sf::Sprite _background;

	sf::Sprite toolWindow;
	sf::RenderTexture toolCanvas;
	int toolWidth;
	int offset;
	bool visibleTools = true;

	void scaleView(int deltaScale);
	void scrollView(sf::Vector2i deltaScroll);

	bool inputField, endMap;

	void toolWindowUpdate();

	sf::Sprite arrowButton;

	std::vector<sf::Sprite> toolSprites;
	int id = 0;

	void initToolSprites();
	
};

