#include "InfoBar.h"



InfoBar::InfoBar(int x, int y, int width, int height, GameDataRef data, sf::Vector2f cameraSize, int startHealth) : _data(data){
	canvasInfo.create(width, height);
	
	_background.setTexture(_data->asset.getTexture("InfoBar"));
	heartTexture = _data->asset.getTexture("Heart");
	
	canvasInfo.display();

	spriteInfo.setTexture(canvasInfo.getTexture());
	spriteInfo.setScale(cameraSize.x/windowWidth, cameraSize.y/ windowHeight);
	_width = spriteInfo.getGlobalBounds().width;
	_height = spriteInfo.getGlobalBounds().height;

	playerHealth.setHealth(startHealth, heartTexture);
	playerHealth.setPlayerHitSound(_data->asset.getSound("Hit player"));
	

	reDrawCanvas();
}

void InfoBar::updateInfoBar(int x, int y) {
	spriteInfo.setPosition(x, y- _height);
}

void InfoBar::drawInfoBar() {
	_data->window.draw(spriteInfo);
}

void InfoBar::reDrawCanvas() {
	canvasInfo.clear(sf::Color::Transparent);
	canvasInfo.draw(_background);
	playerHealth.draw(canvasInfo);
	spriteInfo.setTexture(canvasInfo.getTexture());
}

bool InfoBar::changeHealth(int delta) {
	if (playerHealth.deltaHealth(delta, heartTexture)) {
		return true;
	}
	reDrawCanvas();
	return false;
}

void InfoBar::setHealth(int health) {
	playerHealth.setHealth(health, heartTexture);
	reDrawCanvas();
}