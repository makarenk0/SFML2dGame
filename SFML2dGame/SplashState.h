#pragma once
#include "State.h"
#include "Game.h"
#include "DEFINITIONS.h"
#include "MainMenuState.h"

class SplashState : public State
{
public:
	SplashState(GameDataRef data);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;
	sf::Clock _clock;
	sf::Sprite _background;


};

