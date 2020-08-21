#include "Game.h"
#include "SplashState.h"


Game::Game(int width, int height, std::string name) {
	_data->window.create(sf::VideoMode(width, height), name, sf::Style::Close | sf::Style::Titlebar);
	_data->window.setFramerateLimit(FPS);
	_data->window.setActive(false);  //disable openGL calls
	_data->window.pushGLStates(); //save openGl states
	_data->machine.AddState(StateRef(new SplashState(this->_data)), true);
	
	this->run();
}

void Game::run() {
	float newTime, frameTime, interpolation;
	float currentTime = this->_clock.getElapsedTime().asSeconds();
	float accumulator = 0.0f;

	while (this->_data->window.isOpen()) {
		this->_data->machine.ProcessStateChanges();

		newTime = this->_clock.getElapsedTime().asSeconds();
		
		frameTime = newTime - currentTime;
		
		if (frameTime > 0.25f) {
			frameTime = 0.25f;
		}

		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= dt) {
			this->_data->machine.GetCurrentState()->HandleInput();
			this->_data->machine.GetCurrentState()->Update(dt);
			accumulator -= dt;
		}

		interpolation = accumulator / dt;

		this->_data->machine.GetCurrentState()->Draw(interpolation);
		
	}
}