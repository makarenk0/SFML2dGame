#include "Inventory.h"

Inventory::Inventory(GameDataRef data) : _data(data)
{
	_inventoryData = new int[_inventorySize];
	for (int i = 0; i < _inventorySize; i++) {
		_inventoryData[i] = 0;
	}

	sf::Texture buf = _data->asset.getTexture("Inventory Slot");
	_slotWidth = buf.getSize().x;
	_slotHeigth = buf.getSize().y;
	_canvas.create(_slotWidth * _inventorySize + _rangeBetweenSlots * _inventorySize, _slotHeigth);
	redrawCanvas();
}

Inventory::~Inventory()
{	
	delete[] _inventoryData;
}

void Inventory::putInFreeSlot(int id)
{
	if (areFreeSlots()) {
		for (int i = 0; i < _inventorySize; i++) {
			if (_inventoryData[i] == 0) {
				_inventoryData[i] = id;
				break;
			}
		}
	}
	redrawCanvas();
}

bool Inventory::areFreeSlots()
{
	for (int i = 0; i < _inventorySize; i++) {
		if (_inventoryData[i] == 0) {
			return true;
		}
	}
	return false;
}

bool Inventory::isSuchItem(int id)
{
	for (int i = 0; i < _inventorySize; i++) {
		if (_inventoryData[i] == id) {
			return true;
		}
	}
	return false;
}

void Inventory::removeItem(int id)
{
	for (int i = 0; i < _inventorySize; i++) {
		if (_inventoryData[i] == id) {
			_inventoryData[i] = 0;
			break;
		}
		
	}
	redrawCanvas();
}

void Inventory::update(sf::View& view)
{
	_inventorySprite.setPosition(view.getCenter().x - (view.getSize().x * _canvas.getSize().x/2) / windowWidth, view.getCenter().y + view.getSize().y / 2 - (view.getSize().y * _canvas.getSize().y) / windowHeight);
	_inventorySprite.setScale(view.getSize().x / windowWidth, view.getSize().y / windowHeight);
}



void Inventory::redrawCanvas()
{
	_canvas.clear(sf::Color::Transparent);
	for (int i = 0; i < _inventorySize; i++) {
		sf::Sprite slot(_data->asset.getTexture("Inventory Slot"));
		slot.setPosition(i * _rangeBetweenSlots + i * _slotWidth, 0);
		_canvas.draw(slot);
		if (_inventoryData[i] != 0) {
			int id = _inventoryData[i];
			sf::Sprite item(_data->asset.getTexture("Objects Texture"));
			item.setTextureRect(sf::IntRect((id % 17) * tileSize, (id / 17) * tileSize, tileSize, tileSize));
			item.setPosition(i * _rangeBetweenSlots + i * _slotWidth + 8, 0);
			_canvas.draw(item);
		}
	}
	_inventorySprite.setTexture(_canvas.getTexture());
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_inventorySprite, states);
}
