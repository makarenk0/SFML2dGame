#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "ParticlesSet.h"

class CoinsParticlesSet : public ParticlesSet
{
public:
	CoinsParticlesSet(int x, int y, float lifeTime, float timeOffset, sf::RenderWindow& window);
	virtual void update(float dt);
private:
	const float G = 2000;
	const float _horizontalDistance = 0.2;
	const float _verticalDistance = 0;
	const float _resistence = 50.0f;
	int _startAreaWidth, _startAreaHeight;
	float _vectorVelocity;
	sf::Vector2f _force;
};

