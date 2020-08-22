#pragma once
#include "ParticlesSet.h"

class BloodParticlesSet : public ParticlesSet
{
public:
	BloodParticlesSet(int x, int y, float lifeTime, sf::RenderWindow& window);
	virtual void update(float dt);
private:
	const float G = 30000;
	const float _horizontalDistance = 0.6;
	const float _verticalDistance = 0.6;
	int _startAreaWidth, _startAreaHeight;
};

