#include <SFML\Graphics.hpp>
#include <math.h>
#include <vector>
#pragma once

class Particle
{
private: 
	std::vector <sf::Vector2f> forces;
	sf::Vector2f position;
	sf::Vector2f main_force;
	sf::Vector2f speed;
	int mass;
public:
	Particle();
	Particle(int m, sf::Vector2f pos);
	sf::Vector2f getSpeed()  noexcept { return speed; };
	sf::Vector2f getPosition() const noexcept { return position; }
	sf::Vector2f getMainForce() noexcept;
	void cler_force() noexcept;
	void addForce(sf::Vector2f& n_force) noexcept;
	void update(float&& t) noexcept;
	~Particle();
};
