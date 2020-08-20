#include <SFML\Graphics\Drawable.hpp>
#include <vector>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include "Game.h"
#pragma once
class Inventory : public sf::Drawable
{
public:
	Inventory(GameDataRef data);
	~Inventory();
	void putInFreeSlot(int id);
	bool areFreeSlots();
	bool isSuchItem(int id);
	void removeItem(int id);
	void update(sf::View& view);
private:
	const int _inventorySize = 5;
	const int _rangeBetweenSlots = 10;
	int _slotWidth, _slotHeigth;
	int* _inventoryData;


	void redrawCanvas();
	GameDataRef _data;
	sf::RenderTexture _canvas;
	sf::Sprite _inventorySprite;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

