#include "SplashState.h"



SplashState::SplashState(GameDataRef data) : _data(data)
{
	
}

void SplashState::Init() {
	_data->asset.loadTexture("Splash state background", SPLASH_STATE_BACKGROUND_FILEPATH);
	_background.setTexture(_data->asset.getTexture("Splash state background"));
	_background.setScale(windowWidth/_background.getGlobalBounds().width, windowHeight / _background.getGlobalBounds().height);
}

void SplashState::HandleInput() {
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			_data->window.close();
		}
	}
}

void SplashState::Update(float dt) {
	if (_clock.getElapsedTime().asSeconds() > SPLASH_STATE_TIME) {
		_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
	}
}

void SplashState::Draw(float dt) {
	_data->window.clear();
	_data->window.draw(_background);
	_data->window.display();
}

