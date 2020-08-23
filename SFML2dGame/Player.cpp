#include "Player.h"



Player::Player(std::string textureName, TileMap* map, GameDataRef data, int visibleWidth, int visibleHeight) : Entity(textureName, map, data, 5)
{
	_visibleWidth = visibleWidth;
	_visibleHeight = visibleHeight;
	
	initPlayerCamera();

	playerHealth = 5;
	infoBar = new InfoBar(0,0, 500, 100, _data, playerCamera.getSize(), playerHealth);
	_inventory = new Inventory(_data);
	_map->_playerInventory = _inventory;


	gameOverText.setFont(_data->asset.getFont("MainMenu Font"));
	gameOverText.setFillColor(sf::Color(178, 34, 34));
	gameOverText.setCharacterSize(200);

	victoryText.setFont(_data->asset.getFont("MainMenu Font"));
	victoryText.setFillColor(sf::Color(34, 139, 34));
	victoryText.setCharacterSize(200);


	

}

void Player::updatePlayer(float dt){
		if (gameOver||victory) {
			if (gameOverClock->getElapsedTime().asSeconds() > 0.3) {
				
				gameOverClock->restart();
				if (gameOver) {
					gameOverStringBuf += gameOverString.at(counter);
					gameOverText.setString(gameOverStringBuf);
					if (counter > 10) {
						_data->machine.RemoveState();
						_finishState = true;
					}
				}
				else {
					gameOverStringBuf += victoryString.at(counter);
					victoryText.setString(gameOverStringBuf);
					if (counter > 14) {
						_data->machine.RemoveState();
						_finishState = true;
					}
				}
				counter++;
					
			}
			
		}
		else {


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !fall) {
				jumpPlayer();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				//	dir = 2;
				//	_speed = 200.f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				animationIteratorY = 1;
				dx = -200;
				animatePlayer();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				animationIteratorY = 0;
				dx = 200;
				animatePlayer();
			}

			updateEntity(dt);
			updateCamera(dt);

			if (playerHealth != _entityHealth) {
				_data->particles.addParticlesEffect(new BloodParticlesSet(x + w/3, y+h, 700, 0, _data->window));
				jumpPlayer();
				if (infoBar->changeHealth(_entityHealth - playerHealth))
					y = _map->mapHeight;
				playerHealth = _entityHealth;

			}
			infoBar->updateInfoBar(playerCamera.getCenter().x - _visibleWidth / 2, playerCamera.getCenter().y + _visibleHeight / 2); // TO DO refactor, deriving from sf::Drawable
			_inventory->update(playerCamera);
			checkGameOver();
			checkVictory();
			dx = 0;
		}
}

void Player::checkGameOver() {
	if (y + h > _map->mapHeight - tileSize / 2) {
		infoBar->setHealth(0);
		gameOver = true;
		gameOverText.setPosition(sf::Vector2f(playerCamera.getCenter().x - 110, playerCamera.getCenter().y - 50));
		gameOverText.setScale(playerCamera.getSize().x / windowWidth, playerCamera.getSize().y / windowHeight);
		gameOverClock = new sf::Clock;
		gameOverSound.setBuffer(_data->asset.getSound("Gameover Sound"));
		gameOverSound.play();
	}
}

void Player::checkVictory() {
	if (_map->playerWin) {
		victory = true;
		victoryText.setPosition(sf::Vector2f(playerCamera.getCenter().x - 100, playerCamera.getCenter().y - 50));
		victoryText.setScale(playerCamera.getSize().x / windowWidth, playerCamera.getSize().y / windowHeight);
		gameOverClock = new sf::Clock;
		victorySound.setBuffer(_data->asset.getSound("Victory Sound"));
		victorySound.play();
	}
}

void Player::initPlayerCamera()
{
	cameraBounds = sf::FloatRect(_visibleWidth / 2, _visibleHeight / 2, _map->mapWidth - _visibleWidth, _map->mapHeight - _visibleHeight);
	playerCamera.reset(sf::FloatRect(x - _visibleWidth / 2, y - _visibleHeight / 2, _visibleWidth, _visibleHeight));

	int playerCameraInitX, playerCameraInitY;
	if (playerCamera.getCenter().x - (_visibleWidth / 2) < 0) playerCameraInitX = (_visibleWidth / 2);
	else if (playerCamera.getCenter().x + (_visibleWidth / 2) > _map->mapWidth) {
		playerCameraInitX = _map->mapWidth - (_visibleWidth / 2);
	}
	else playerCameraInitX = x;

	if (playerCamera.getCenter().y - (_visibleHeight / 2) < 0) playerCameraInitY = (_visibleHeight / 2);
	else if (playerCamera.getCenter().y + (_visibleHeight / 2) > _map->mapHeight) {
		playerCameraInitY = _map->mapHeight - (_visibleHeight / 2);
	}
	else playerCameraInitY = y;

	playerCamera.setCenter(playerCameraInitX, playerCameraInitY);

	_map->mapSpritePtr->setTextureRect(sf::IntRect(playerCameraInitX - _visibleWidth / 2, playerCameraInitY - _visibleHeight / 2, _visibleWidth, _visibleHeight));
	_map->mapSpritePtr->setPosition(playerCameraInitX - _visibleWidth / 2, playerCameraInitY - _visibleHeight / 2);
	_map->backgroundPtr->setPosition(playerCameraInitX - _visibleWidth / 2, 0);
}

void Player::animatePlayer() {
	if (clock.getElapsedTime().asMilliseconds() > PLAYER_ANIMATION_DURATION) {
		if (animationIteratorX < PLAYER_ANIMATION_FRAMES-1) {
			animationIteratorX++;
		}
		else {
			animationIteratorX = 0;
		}
		entity._entity.setTextureRect(sf::IntRect(animationIteratorX * PLAYER_TEXTURE_WIDTH, animationIteratorY*PLAYER_TEXTURE_HEIGHT, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
		clock.restart();
	}
}

void Player::drawPlayer() {
	drawEntity();
	infoBar->drawInfoBar();
	_data->window.draw(*_inventory);
}

void Player::updateCamera(float dt) {
	if (cameraBounds.contains(x, _visibleHeight/2)) {
		playerCamera.setCenter(x, playerCamera.getCenter().y);
		_map->mapSpritePtr->setTextureRect(sf::IntRect(x - _visibleWidth / 2, _map->mapSpritePtr->getTextureRect().top, _visibleWidth, _visibleHeight));
		_map->mapSpritePtr->setPosition(x - _visibleWidth / 2, _map->mapSpritePtr->getPosition().y);
		_map->backgroundPtr->setPosition(x - _visibleWidth / 2, 0);
		_map->mapAnimationsUpdate(dt, dx, x);
	}
	else {
		_map->mapAnimationsUpdate(dt, 0, x);
	}
	if (cameraBounds.contains(_visibleWidth/2, y)) {
		playerCamera.setCenter(playerCamera.getCenter().x, y);
		_map->mapSpritePtr->setTextureRect(sf::IntRect(_map->mapSpritePtr->getTextureRect().left, y - _visibleHeight / 2, _visibleWidth, _visibleHeight));
		_map->mapSpritePtr->setPosition(_map->mapSpritePtr->getPosition().x, y - _visibleHeight / 2);
	}
	

	_data->window.setView(playerCamera);
}

void Player::drawGameOver() {
	if (gameOver) {
		_data->window.draw(gameOverText);
	}
	else {
		_data->window.draw(victoryText);
	}
	
}

void Player::jumpPlayer() {
	moveEntity(0, -1);
	jumpAcc = 2.5;
}
