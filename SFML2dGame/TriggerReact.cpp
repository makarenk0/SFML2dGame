#include "TriggerReact.h"



TriggerReact::TriggerReact(std::tuple<int,int, sf::IntRect> trigger, double frameTime): _trigger(trigger), _frameTime(frameTime) {
	triggerLife = new sf::Clock;
	x = (std::get<2>(_trigger).left / tileSize) * tileSize;
	y = (std::get<2>(_trigger).top / tileSize) * tileSize;
}

TriggerReact::~TriggerReact()
{
}

void TriggerReact::update() {
	if (triggerLife->getElapsedTime().asSeconds() > _frameTime) {
		_updateData = std::vector<int>({ x , y, std::get<0>(_trigger) + _counter * objTextureWidth, 0 });
		triggerLife->restart();
		if (_counter == std::get<1>(_trigger)-1) {
			delete triggerLife;
			_remove = true;
		}
		_counter++;
		_update = true;
	}
}

bool TriggerReact::getOnUpdate()
{
	if (_update) {
		_update = false;
		return true;
	}
	return false;
}

bool TriggerReact::getOnRemove()
{
	return _remove;
}
