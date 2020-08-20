#include "InputManager.h"
#include <iostream>



bool InputManager::isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect clickRect(sprite.getGlobalBounds().left, sprite.getGlobalBounds().top, sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
		if (clickRect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}

bool InputManager::isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window, sf::Sprite relativeTo, sf::View view) {
	if (sf::Mouse::isButtonPressed(button)) {
		
		sf::IntRect clickRect(relativeTo.getGlobalBounds().left + sprite.getGlobalBounds().left -view.getCenter().x+view.getSize().x/2, relativeTo.getGlobalBounds().top + sprite.getGlobalBounds().top -view.getCenter().y + view.getSize().y/2,sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
		if (clickRect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}

bool InputManager::isRectClicked(sf::IntRect rect, sf::Mouse::Button button, sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect clickRect(rect.left, rect.top, rect.width, rect.height);
		if (clickRect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}

bool InputManager::isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window, sf::View view) {
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect clickRect((sprite.getGlobalBounds().left - view.getCenter().x + view.getSize().x / 2) / view.getSize().x * windowWidth, ( sprite.getGlobalBounds().top - view.getCenter().y + view.getSize().y / 2)/view.getSize().y * windowHeight, sprite.getGlobalBounds().width / view.getSize().x * windowWidth, sprite.getGlobalBounds().height / view.getSize().y * windowHeight);
		if (clickRect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}

bool InputManager::isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window, sf::Sprite relativeTo, sf::View view, float coef) {
	
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect clickRect(sprite.getGlobalBounds().left, sprite.getGlobalBounds().top + coef, sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
		
		if (clickRect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}



bool InputManager::isTitleClicked(sf::Text text, sf::Mouse::Button button, sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect clickRect(text.getPosition().x, text.getPosition().y+25, text.getGlobalBounds().width, text.getGlobalBounds().height);
		if (clickRect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}

bool InputManager::isMouseInRect(sf::IntRect rect, sf::RenderWindow& window)
{
	if (rect.contains(sf::Mouse::getPosition(window))) {
		return true;
	}
	return false;
}

sf::Vector2i InputManager::getMousePosition(sf::RenderWindow& window) {
	return sf::Mouse::getPosition(window);
}

