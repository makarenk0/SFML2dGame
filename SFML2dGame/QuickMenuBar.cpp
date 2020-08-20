#include "QuickMenuBar.h"
#include <iostream>







QuickMenuBar::QuickMenuBar(GameDataRef data) : _data(data)
{
	canvas.create(quickMenuSpriteW, quickMenuSpriteH);
	canvas.clear(sf::Color(238, 130, 238));
	
	playButton = sf::Sprite(_data->asset.getTexture("Play Button"));
	playButton.setPosition(5, 10);
	canvas.draw(playButton);


	quickMenuSprite.setTexture(canvas.getTexture());
	canvas.display();

}

void QuickMenuBar::update(sf::View& view)
{
	quickMenuSprite.setPosition(view.getCenter().x + view.getSize().x/2 - (view.getSize().x * quickMenuSpriteW)/windowWidth, view.getCenter().y - view.getSize().y/2);
	quickMenuSprite.setScale(view.getSize().x / windowWidth, view.getSize().y / windowHeight);
}

bool QuickMenuBar::isPlayButtonClicked(sf::View& view)
{
	return _data->input.isRectClicked(sf::IntRect(windowWidth - quickMenuSpriteW + playButton.getGlobalBounds().left, playButton.getGlobalBounds().top, playButton.getGlobalBounds().width, playButton.getGlobalBounds().height), sf::Mouse::Left, _data->window);
}

void QuickMenuBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(quickMenuSprite, states);	
}