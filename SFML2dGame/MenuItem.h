#pragma once
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

class MenuItem : public sf::Drawable
{
public:
	MenuItem();
	MenuItem(const std::string& text, const sf::Font& font, const int& fontSize, const sf::Color& color, const sf::Color& onChangeColor);
	MenuItem(const std::string& text, const sf::Font& font, const int& fontSize, const sf::Color& color, const sf::Color& onChangeColor, const int& startX, const int& startY);
	void setText(std::string& text);
	void setPosition(const int& x, const int& y);
	void setVisible();
	void setTransparent();
	void setTransform(const int& dx, const int& dy);
	sf::IntRect getRect();
	const std::string getText();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Text _text;
	sf::Color _onChangeColor;
	sf::RectangleShape _rectangle;
};

