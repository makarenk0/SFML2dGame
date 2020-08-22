#include "ParticlesSystem.h"


float ParticlesSystem::modOfVector(sf::Vector2f& v) { // Находим модуль вектора
	return sqrt(v.x * v.x + v.y * v.y);
}

void ParticlesSystem::particleTransform(std::vector <Particle>const& part, float**& i_vec, int numb_of_block, int part_in_block) { // Функция переписывает координаты частиц из вектора, в двумерный массив
	for (int i = 0; i < numb_of_block; ++i) {
		for (int j = 0; j < part_in_block; ++j) {
			i_vec[i][j * 2] = part[j + i * part_in_block].getPosition().x;
			i_vec[i][j * 2 + 1] = part[j + i * part_in_block].getPosition().y;
		}
	}
}


/*
void ParticlesSystem::testParticles(sf::RenderWindow& window) {
	
	window.popGLStates();
	window.setActive(true);

	std::vector <Particle> all_part; // Вектор частиц

	float** coords = new float* [NUMB_OF_BLOCK]; // Двумерный динамический массив координат точек, используется 

	for (int i = 0; i < NUMB_OF_BLOCK; ++i)
		coords[i] = new float[PART_IN_BLOCK * 2];

	unsigned char** colors = new unsigned char* [NUMB_OF_BLOCK]; // Аналогичный массив, хранящий информацию о цвете каждой точки
	for (int i = 0; i < NUMB_OF_BLOCK; ++i)
		colors[i] = new unsigned char[PART_IN_BLOCK * 3];

	for (int i = 0; i < NUMB_OF_BLOCK; i++) {
		for (int j = 0; j < PART_IN_BLOCK; j++) {// sf::Vector2f(200, 100) - смешение для точек 
			srand(time(0));
			int mass = 10 + rand() % 70; // Выбираем случайую массу в диапазоне от 10 до 79
			Particle tmp(mass, sf::Vector2f(20, 100) + sf::Vector2f(j * 0.6, i * 3)); // Создаем новую частицу
			all_part.push_back(tmp);
			coords[i][2 * j] = tmp.getPosition().x; // Добавляем координаты частицы в массив
			coords[i][2 * j + 1] = tmp.getPosition().y;
		}
	}

	////////////////// OpenGL settings
	glEnable(GL_POINT_SMOOTH); //Три функции приводят нашу квадратную точку к круглой
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(1); // Размер точки - 1 пиксель
	glOrtho(0, windowWidth, windowHeight, 0, 1, -1); // Ориентирование нашей сцены
	//////////////////////////////////

	bool LB_press = 0;

	sf::Clock clock; // Часы, чтобы замерять сколько времени уходит на обработку каждого кадра
	float frame_time = 0.1f;

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))// Тригер для нажатия ЛКМ
			LB_press = 1;
		else
			LB_press = 0;

		glClear(GL_COLOR_BUFFER_BIT); // Очистка буфера

		for (auto it = 0; it < all_part.size(); ++it) {
			if (LB_press) { // Если нажата ЛКМ, то добавляем силы к частицам
				sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window)); // Записываем координаты мышки относительно нашего окна
				//sf::Vector2f vec1((mouse - all_part[it].getPosition()) * (G / pow(getDistance(std::move(mouse), all_part[it].getPosition()) + 5, 2)));
				sf::Vector2f vec2(-all_part[it].getSpeed() * 100.0f);
				//all_part[it].addForce(vec1);
				all_part[it].addForce(vec2); // Тормозящая сила (сила сопротивления), вместе 100*f можно подставить любую магическую константу, чем больше - тем меньшее ускорение у частиц.
			}
			//Обновляем частицу
			all_part[it].update(std::move(frame_time)); // Передает в update информацию о том, сколько времени ушло для вычисления всей информации на предыдущем кадре
			if (LB_press) all_part[it].cler_force(); // Очищаем вектор скоростей

		}

		glPushMatrix();

		//particleTransform(all_part, coords); // Переносим обновленные данные из вектора в массив

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);


		for (int i = 0; i < NUMB_OF_BLOCK; ++i) {
			for (int j = 0; j < PART_IN_BLOCK; ++j) { // Цвета подобраны почти случайно, каждый может менять как горазд
				colors[i][3 * j] = 152;
				sf::Vector2f vec1(all_part[j + i * PART_IN_BLOCK].getSpeed());
				sf::Vector2f vec2(all_part[j + i * PART_IN_BLOCK].getSpeed());
				colors[i][3 * j + 1] = 50 + static_cast <int> (modOfVector(vec1)); // Находим модуль вектора скорости
				colors[i][3 * j + 2] = 10 + static_cast <int> (modOfVector(vec2));
			}
		}

		for (int i = 0; i < NUMB_OF_BLOCK; ++i) {
			glVertexPointer(2, GL_FLOAT, 0, coords[i]); // Передаем кол-во координат точки, формат координат и указатель на массив координат
			glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors[i]); // Тоже самое, только для цвета.
			glDrawArrays(GL_POINTS, 0, PART_IN_BLOCK); // Функция отображения массива элементов. Передаем тип отображаемого объекта, смещение до следующего элемента (у нас 0, т.к. массив линейный) и размер массива.

		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glPopMatrix();

		glFlush();
		window.display(); // Отображаем кадр.
		frame_time = clock.restart().asSeconds(); // Записываем прошедшее время и перезапускаем часы.

	}
	for (int i = 0; i < NUMB_OF_BLOCK; ++i) // Освобождаем динамическую память.
		delete[] coords[i];
	for (int i = 0; i < NUMB_OF_BLOCK; ++i) // Освобождаем динамическую память.
		delete[] colors[i];
}
*/

/*
void ParticlesSystem::addBloodEffect(int x, int y, int amount, float time, sf::RenderWindow& window)
{
	window.popGLStates();
	window.setActive(true);

	//ParticlesSet* newSet = new BloodParticlesSet(10, 100, 2000);

	for (int i = 0; i < newSet->_numb_of_block; i++) {
		for (int j = 0; j < newSet->_part_in_block; j++) {// sf::Vector2f(200, 100) - смешение для точек 
			//srand(time(0));
			int mass = 10 + rand() % 70; // Выбираем случайую массу в диапазоне от 10 до 79
			Particle tmp(mass, sf::Vector2f(20, 100) + sf::Vector2f(j * 0.6, i * 3)); // Создаем новую частицу
			newSet->all_part.push_back(tmp);
			newSet->coords[i][2 * j] = tmp.getPosition().x; // Добавляем координаты частицы в массив
			newSet->coords[i][2 * j + 1] = tmp.getPosition().y;
		}
	}

	////////////////// OpenGL settings
	glEnable(GL_POINT_SMOOTH); //Три функции приводят нашу квадратную точку к круглой
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(4); // Размер точки - 1 пиксель
	glOrtho(0, windowWidth, windowHeight, 0, 1, -1); // Ориентирование нашей сцены
	//////////////////////////////////
	_all_sets.push_back(newSet);

	window.setActive(false);
	window.pushGLStates();
	
}
*/

/*
void ParticlesSystem::updateBloodEffect()
{
	//for (auto it = 0; it < _all_sets[0]->all_part.size(); ++it) {
	//	sf::Vector2f mouse = sf::Vector2f(500, 200); // Записываем координаты мышки относительно нашего окна
	//	sf::Vector2f vec1((mouse - _all_sets[0]->all_part[it].getPosition()) * (G / pow(getDistance(std::move(mouse), _all_sets[0]->all_part[it].getPosition()) + 5, 2)));
	//	sf::Vector2f vec2(-_all_sets[0]->all_part[it].getSpeed() * 100.0f);
	//	_all_sets[0]->all_part[it].addForce(vec1);
	//	_all_sets[0]->all_part[it].addForce(vec2);
	//
	//	_all_sets[0]->all_part[it].update(std::move(0.1)); // Передает в update информацию о том, сколько времени ушло для вычисления всей информации на предыдущем кадре
	////all_part[it].cler_force(); // Очищаем вектор скоростей

	//}
} */

void ParticlesSystem::addParticlesEffect(ParticlesSet* p_set)
{
	_all_sets.push_back(p_set);
}

void ParticlesSystem::update(sf::RenderWindow& window, float dt)
{
	for (int i = 0; i < _all_sets.size(); i++) {
		_all_sets[i]->update(dt);
		if (_all_sets[i]->_lifeTimer.getElapsedTime().asMilliseconds() > _all_sets[i]->_lifeTime) {
			ParticlesSet* buf = _all_sets[i];
			_all_sets.erase(_all_sets.begin() + i);
			delete buf;
		}
	}
}

void ParticlesSystem::draw(sf::RenderWindow& window)
{
	window.popGLStates();
	window.setActive(true);

	for (auto& set : _all_sets) {
		glPushMatrix();

		particleTransform(set->all_part, set->coords, set->_numb_of_block, set->_part_in_block); // Переносим обновленные данные из вектора в массив

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		for (int i = 0; i < set->_numb_of_block; ++i) {
			glVertexPointer(2, GL_FLOAT, 0, set->coords[i]); // Передаем кол-во координат точки, формат координат и указатель на массив координат
			glColorPointer(3, GL_UNSIGNED_BYTE, 0, set->colors[i]); // Тоже самое, только для цвета.
			glDrawArrays(GL_POINTS, 0, set->_part_in_block); // Функция отображения массива элементов. Передаем тип отображаемого объекта, смещение до следующего элемента (у нас 0, т.к. массив линейный) и размер массива.

		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glPopMatrix();

		glFlush();
	}
	window.setActive(false);
	window.pushGLStates();
}

