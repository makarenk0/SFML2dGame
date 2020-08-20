#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\View.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include "DEFINITIONS.h"
#include "Game.h"

class QuickMenuBar : public sf::Drawable
{
public:
	QuickMenuBar(GameDataRef data);
	void update(sf::View& view);
	bool isPlayButtonClicked(sf::View& view);
private:
	const int quickMenuSpriteH = 200, quickMenuSpriteW = 60;
	sf::RenderTexture canvas;
	sf::Sprite quickMenuSprite;

	sf::Sprite playButton;
	GameDataRef _data;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

