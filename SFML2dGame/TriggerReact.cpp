#include "TriggerReact.h"

TriggerReact::TriggerReact(TileMap* map, std::tuple<int,int, sf::IntRect> trigger, int frameTime):_map(map), _trigger(trigger), _frameTime(frameTime) {
	triggerLife = new sf::Clock;
}

void TriggerReact::update() {
	if (triggerLife->getElapsedTime().asSeconds() > _frameTime) {
		_map->changeTile(std::get<2>(_trigger).left, std::get<2>(_trigger).top, std::get<0>(_trigger) + _counter * objTextureWidth, false);
		triggerLife->restart();
		if (_counter == std::get<1>(_trigger)-1) {
			_remove = true;
		}
		_counter++;
	}
}