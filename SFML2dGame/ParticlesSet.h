#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>
#include "Particle.h"
#include <SFML/OpenGL.hpp>
#include "DEFINITIONS.h"

class ParticlesSet
{
public:
	ParticlesSet(int x, int y, int numb_of_block, int part_in_block, float lifeTime, float timeOffset);
	~ParticlesSet();
	virtual void update(float dt) = 0;

	sf::Clock _lifeTimer;
	float _lifeTime;
	float _timeOffset;

	std::vector <Particle> all_part;
	float** coords;
	unsigned char** colors;

	int _numb_of_block;
	int _part_in_block;

protected:
	float getDistance(sf::Vector2f&& v1, sf::Vector2f&& v2) noexcept;
	int _x, _y;
	

	

};

