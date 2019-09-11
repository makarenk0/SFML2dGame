#pragma once
#include "State.h"
#include "Game.h"
#include "DEFINITIONS.h"
#include <iostream>

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
	GameDataRef _data;

	sf::Sound menuMusic;
	sf::Sprite _background;
	sf::Text _singlePlayerText;
	sf::Text _mapEditorText;

	sf::Sprite menuSprite;
	sf::RenderTexture menuCanvas;
	sf::View menuView;


};
