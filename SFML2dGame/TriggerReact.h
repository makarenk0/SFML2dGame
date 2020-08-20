#pragma once
#include <tuple>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Clock.hpp>
#include <vector>
#include "DEFINITIONS.h"

class TriggerReact {
public:
	TriggerReact(std::tuple<int , int, sf::IntRect> trigger, double frameTime);
	~TriggerReact();
	void update();
	bool getOnUpdate();
	bool getOnRemove();
	std::vector<int> _updateData;
	int x, y;
private:
	std::tuple<int,int, sf::IntRect> _trigger;
	sf::Clock* triggerLife;
	bool _remove = false;
	bool _update = false;
	double _frameTime;
	int _counter=1;
	const int objTextureWidth = 17;
	
};