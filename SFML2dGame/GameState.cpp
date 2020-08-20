#include "GameState.h"


GameState::GameState(GameDataRef data, std::string mapName, bool campaignMode) : _data(data), _mapName(mapName), _campaignMode(campaignMode)
{
	
}

GameState::~GameState()
{
	delete player;
	delete map;
}

void GameState::Init() {
	_data->asset.loadTexture("Player Texture", PLAYER_TEXTURE_FILEPATH);
	int visibleWidth= windowWidth/3, visibleHeight = windowHeight/3;
	map = new TileMap(_data, visibleWidth, visibleHeight, _mapName, _campaignMode);
	player = new Player("Player Texture", map, _data, visibleWidth, visibleHeight);
	
}

void GameState::HandleInput() {
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		
			switch (event.type)
			{
			case sf::Event::Closed:
				_data->window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					finishState = true;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
					break;
				}
				break;
				
			}
	}
	
}

void GameState::Update(float dt) {
	player->updatePlayer(dt);
}

void GameState::Draw(float dt) {
	_data->window.clear();
	_data->window.draw(*map);
	player->drawPlayer();
	if (player->gameOver||player->victory) {
		player->drawGameOver();
	}

	_data->window.display();

	if (finishState||player->_finishState) {
		_data->machine.RemoveState();
	}
}


