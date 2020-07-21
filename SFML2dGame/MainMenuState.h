#pragma once
#include "State.h"
#include "Game.h"
#include "MenuItem.h"
#include "DEFINITIONS.h"
#include <iostream>
#include <list>
#include <filesystem>
namespace fs = std::filesystem;

class MainMenuState : public State
{
public:
	MainMenuState(GameDataRef data);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

	void Pause();
	void Resume();

private:
	int rangeBetweenButtons;
	GameDataRef _data;

	sf::Sound menuMusic;
	sf::Sprite _background;

	sf::Sprite menuSprite;
	sf::RenderTexture menuCanvas;
	sf::View menuView;
	int _state;
	void menuCanvasRedraw();
	void menuNavigate();

	std::vector<MenuItem> currentMenuList;
	std::map<std::string, std::vector<MenuItem>> menuListData;

};
