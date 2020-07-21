#include "Player.h"



Player::Player(std::string textureName, TileMap* map, GameDataRef data, int visibleWidth, int visibleHeight) : Entity(textureName, map, data, 5)
{
	_visibleWidth = visibleWidth;
	_visibleHeight = visibleHeight;
	cameraBounds = sf::FloatRect(visibleWidth/2, visibleHeight/2, _map->mapWidth- visibleWidth, _map->mapHeight - visibleHeight);
	playerCamera.reset(sf::FloatRect(x- visibleWidth /2, y - visibleHeight / 2, visibleWidth, visibleHeight));


	playerHealth = 5;
	infoBar = new InfoBar(0,0, 500, 100, _data, playerCamera.getSize(), playerHealth);


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
				jumpPlayer();
				if (infoBar->changeHealth(_entityHealth - playerHealth))
					y = _map->mapHeight;
				playerHealth = _entityHealth;

			}
			infoBar->updateInfoBar(playerCamera.getCenter().x - _visibleWidth / 2, playerCamera.getCenter().y + _visibleHeight / 2);
			checkTriggers();
			_map->canvasUpdate(x);
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

void Player::checkTriggers() { 
	for (auto& i : _map->triggers) {
		if (entity.intersects(std::get<2>(i))&& sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			triggersBuffer.push_back(TriggerReact(_map, i , 0.5));
			_map->triggers.remove(i);
			break;
		}
	}
	for (auto& i : triggersBuffer) {
		i.update();
	}
	if (!triggersBuffer.empty()&&triggersBuffer.front()._remove) {
		triggersBuffer.pop_front();
	}
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
