#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "StateMachine.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "ParticlesSystem.h"



struct GameData {
	StateMachine machine;
	sf::RenderWindow window;
	AssetManager asset;
	InputManager input;
	ParticlesSystem particles;
};
typedef std::shared_ptr<GameData> GameDataRef;
class Game
{
public:
	Game(int width, int height, std::string name);
private:
	const float dt = 1.0f / 60.0f;
	sf::Clock _clock;

	GameDataRef _data = std::make_shared<GameData>();
	void run();
};

