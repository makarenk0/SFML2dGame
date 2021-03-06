#include "BloodParticlesSet.h"



BloodParticlesSet::BloodParticlesSet(int x, int y, float lifeTime, float timeOffset, sf::RenderWindow& window) : ParticlesSet(x, y, 20, 20, lifeTime, timeOffset) {
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
			colors[i][3 * j + 1] = 0;
			colors[i][3 * j + 2] = 0;
		}
	}

	// OpenGL settings
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(3); 
	glOrtho(0, windowWidth, windowHeight, 0, 1, -1);


	window.setActive(false);
	window.pushGLStates();
}

void BloodParticlesSet::update(float dt) {
	for (auto it = 0; it < all_part.size(); ++it) {
		sf::Vector2f mouse = sf::Vector2f(_x + _startAreaWidth/2 + 1, _y + 2); // Записываем координаты мышки относительно нашего окна
		sf::Vector2f vec1(-(mouse - all_part[it].getPosition()) * (G / pow(getDistance(std::move(mouse), all_part[it].getPosition()) + 5, 2)));
		sf::Vector2f vec2(-all_part[it].getSpeed() * _resistence);
		all_part[it].addForce(vec1);
		all_part[it].addForce(vec2);


		all_part[it].update(std::move(0.1)); // Передает в update информацию о том, сколько времени ушло для вычисления всей информации на предыдущем кадре
	//all_part[it].cler_force(); // Очищаем вектор скоростей

	}
}