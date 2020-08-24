#include "InfoBar.h"



InfoBar::InfoBar(int visibleWidth, int visibleHeigth, int width, int height, GameDataRef data, sf::Vector2f cameraSize, int startHealth, int overallCheatsNumber) : _data(data), _visibleWidth(visibleWidth), _visibleHeigth(visibleHeigth){
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
	
	_overallCheastsNumber = overallCheatsNumber;
	initCheatsCounter();
	reDrawCanvas();
}

void InfoBar::updateInfoBar(int x, int y) {
	spriteInfo.setPosition(x + _visibleWidth / 2 - spriteInfo.getGlobalBounds().width / 2, y);
}

void InfoBar::reDrawCanvas() {
	canvasInfo.clear(sf::Color::Transparent);
	canvasInfo.draw(_background);
	canvasInfo.draw(_cheastsCounter);
	playerHealth.draw(canvasInfo);
	spriteInfo.setTexture(canvasInfo.getTexture());
}

void InfoBar::initCheatsCounter()
{
	_cheastsCounter = sf::Text();
	_cheastsCounter.setFont(_data->asset.getFont("MainMenu Font"));
	_cheastsCounter.setString("0/" + std::to_string(_overallCheastsNumber));
	_cheastsCounter.setCharacterSize(30);
	_cheastsCounter.setFillColor(sf::Color::White);
	_cheastsCounter.setPosition(310, 18);
}

void InfoBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(spriteInfo, states);
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

void InfoBar::increaseCheastsCounter()
{
	++_currentCheastsPicked;
	_cheastsCounter.setString(std::to_string(_currentCheastsPicked)+ "/" + std::to_string(_overallCheastsNumber));
	reDrawCanvas();
}
