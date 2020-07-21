#include "MenuItem.h"

MenuItem::MenuItem()
{
}

MenuItem::MenuItem(const std::string& text, const sf::Font& font, const int& fontSize, const sf::Color& color, const sf::Color& onChangeColor) : _onChangeColor(onChangeColor)
{
	_text = sf::Text();
	_text.setFont(font);
	_text.setString(text);
	_text.setCharacterSize(fontSize);
	_text.setFillColor(color);

	_rectangle.setSize(sf::Vector2f(_text.getGlobalBounds().width + 20, _text.getGlobalBounds().height + 20));
	_rectangle.setFillColor(sf::Color(0, 0, 0, 0));
}

MenuItem::MenuItem(const std::string& text, const sf::Font& font, const int& fontSize, const sf::Color& color, const sf::Color& onChangeColor, const int& startX, const int& startY) : _onChangeColor(onChangeColor)
{
	_text = sf::Text();
	_text.setFont(font);
	_text.setString(text);
	_text.setCharacterSize(fontSize);
	_text.setFillColor(color);

	_rectangle.setSize(sf::Vector2f(_text.getGlobalBounds().width + 20, _text.getGlobalBounds().height + 20));
	_rectangle.setFillColor(sf::Color(0, 0, 0, 0));

	setPosition(startX, startY);
}

void MenuItem::setText(std::string& text)
{
	_text.setString(text);
}

void MenuItem::setPosition(const int& x, const int& y)
{
	_text.setPosition(x, y);
	_rectangle.setPosition(x-10, y);
}

void MenuItem::setVisible()
{
	_rectangle.setFillColor(_onChangeColor);
}

void MenuItem::setTransparent()
{
	_rectangle.setFillColor(sf::Color(0, 0, 0, 0));
}

sf::IntRect MenuItem::getRect()
{
	return sf::IntRect(_rectangle.getGlobalBounds().left, _rectangle.getGlobalBounds().top, _rectangle.getGlobalBounds().width, _rectangle.getGlobalBounds().height);
}

const std::string MenuItem::getText()
{
	return _text.getString();
}

void MenuItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rectangle, states);
	target.draw(_text, states);
}
