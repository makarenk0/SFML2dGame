#pragma once
#include <SFML/Graphics.hpp>
#include "DEFINITIONS.h"
class InputManager
{
public:

	bool isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window);
	bool isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window, sf::View view);
	bool isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window, sf::Sprite relativeTo, sf::View view);
	bool isTitleClicked(sf::Text text, sf::Mouse::Button button, sf::RenderWindow& window);
	sf::Vector2i getMousePosition(sf::RenderWindow& window);
};

