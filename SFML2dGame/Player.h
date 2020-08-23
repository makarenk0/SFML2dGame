#pragma once
#include "Entity.h"
#include "TriggerReact.h"
#include "InfoBar.h"
#include "Inventory.h"
#include "BloodParticlesSet.h"

class Player : public Entity
{
public:
	Player(std::string textureName, TileMap* map, GameDataRef data, int visibleWidth, int visibleHeight);
	void updatePlayer(float dt);
	sf::View playerCamera;
	void drawGameOver();
	bool gameOver = false;
	bool victory = false;
	bool _finishState = false;
	void drawPlayer();
private:
	InfoBar* infoBar;
	Inventory* _inventory;

	sf::Clock clock;
	sf::FloatRect cameraBounds;
	int _visibleWidth, _visibleHeight;
	int counter = 0;
	std::string gameOverString = "Game Over    ";
	std::string gameOverStringBuf="";
	sf::Text gameOverText;

	std::string victoryString = "Victory!!!       ";
	sf::Text victoryText;

	void initPlayerCamera();
	void animatePlayer();
	int animationIteratorX = 0, animationIteratorY = 0;
	void updateCamera(float dt);
	sf::Clock* gameOverClock = nullptr;
	int playerHealth;
	void checkGameOver();
	void checkVictory();
	void jumpPlayer();

	sf::Sound gameOverSound;
	sf::Sound victorySound;
	
};

