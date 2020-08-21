#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "DEFINITIONS.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

//using namespace std;
using std::byte;

class ParticlesSystem
{
private:
	float modOfVector(sf::Vector2f& v);
	float getDistance(sf::Vector2f&& v1, sf::Vector2f&& v2) noexcept;
	void particleTransform(std::vector <Particle>const& part, float**& i_vec);
public:
	void testParticles(sf::RenderWindow& window); //sf::RenderWindow& window
};

