#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "DEFINITIONS.h"
#include <SFML/Window.hpp>
#include <list>
#include "ParticlesSet.h"


using std::byte;

class ParticlesSystem
{
private:
	float modOfVector(sf::Vector2f& v);
	void particleTransform(std::vector<Particle>const& part, float**& i_vec, int numb_of_block, int part_in_block);
	std::vector<ParticlesSet*> _all_sets;

public:

	
	//void testParticles(sf::RenderWindow& window); //sf::RenderWindow& window

	void addParticlesEffect(ParticlesSet* p_set);
	void update(sf::RenderWindow& window, float dt);
	void draw(sf::RenderWindow& window);
};

