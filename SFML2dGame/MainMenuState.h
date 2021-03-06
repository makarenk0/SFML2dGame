#pragma once
#include <iostream>
#include <list>
#include <filesystem>
#include <thread>

#include "State.h"
#include "Game.h"
#include "MenuItem.h"
#include "DEFINITIONS.h"
#include "ParticlesSystem.h"
#include "BloodParticlesSet.h"

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

	int currentMenuListLength = 0;
	int currentScrollState = 0;
	const int scrollSpeed = 35;
	

	sf::View menuView;
	int _state;
	float _dt;

	std::thread _parallelParticles;

	void menuCanvasRedraw();
	void menuNavigate();
	void computeMenuListLength();
	void scrollMenuListItems(const int& wheelDelta);
	void loadSizesVariants();
	void loadMapsLists();
	void initMenuSection(const std::string& menuSectionName, std::vector<std::string> items);
	void particlesUpdateParallelThread();

	std::vector<MenuItem> currentMenuList;
	std::map<std::string, std::vector<MenuItem>> menuListData;

};
