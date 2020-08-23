#include "CoinsParticlesSet.h"

CoinsParticlesSet::CoinsParticlesSet(int x, int y, float lifeTime, float timeOffset, sf::RenderWindow& window) : ParticlesSet(x, y, 20, 20, lifeTime, timeOffset)
{
	window.popGLStates();
	window.setActive(true);
	_startAreaWidth = _part_in_block * _horizontalDistance;
	_startAreaHeight = _numb_of_block * _verticalDistance;

	srand(time(0));
	for (int i = 0; i < _numb_of_block; i++) {
		for (int j = 0; j < _part_in_block; j++) {// sf::Vector2f(200, 100) - смешение для точек 

			int mass = 10 + rand() % 2000;
			Particle tmp(mass, sf::Vector2f(_x, _y) + sf::Vector2f(j * _horizontalDistance, i * _verticalDistance)); // Создаем новую частицу
			all_part.push_back(tmp);
			coords[i][2 * j] = tmp.getPosition().x; // Добавляем координаты частицы в массив
			coords[i][2 * j + 1] = tmp.getPosition().y;

			colors[i][3 * j] = 255;
			colors[i][3 * j + 1] = 255;
			colors[i][3 * j + 2] = 0;
		}
	}

	// OpenGL settings
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(8);
	glOrtho(0, windowWidth, windowHeight, 0, 1, -1);


	window.setActive(false);
	window.pushGLStates();

	_force = sf::Vector2f(_x + _startAreaWidth / 2, _y + 12);
}

void CoinsParticlesSet::update(float dt)
{
	for (auto it = 0; it < all_part.size(); ++it) {
		if (all_part[it].getPosition().y < _y + 10) {
			sf::Vector2f vec1;
			if (all_part[it].getPosition().y < _y-1) {
				all_part[it].cler_force();
				sf::Vector2f goDown(all_part[it].getPosition().x, all_part[it].getPosition().y - 5);
				vec1 = (-(goDown - all_part[it].getPosition()) * (G / pow(getDistance(std::move(goDown), all_part[it].getPosition()) + 5, 2)));
			}
			else {
				vec1 = (-(_force - all_part[it].getPosition()) * (G / pow(getDistance(std::move(_force), all_part[it].getPosition()) + 5, 2)));	
			}	
			all_part[it].addForce(vec1);
			all_part[it].update(std::move(0.1));
		}

	}
	_force = sf::Vector2f(_force.x, _force.y - 0.5);
}
