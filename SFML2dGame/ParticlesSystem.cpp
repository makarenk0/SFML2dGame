#include "ParticlesSystem.h"


float ParticlesSystem::modOfVector(sf::Vector2f& v) { // Находим модуль вектора
	return sqrt(v.x * v.x + v.y * v.y);
}
float ParticlesSystem::getDistance(sf::Vector2f&& v1, sf::Vector2f&& v2) noexcept { // Растояние между двумя точками
	return sqrt(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2));
}

void ParticlesSystem::particleTransform(std::vector <Particle>const& part, float**& i_vec) { // Функция переписывает координаты частиц из вектора, в двумерный массив
	for (int i = 0; i < NUMB_OF_BLOCK; ++i) {
		for (int j = 0; j < PART_IN_BLOCK; ++j) {
			i_vec[i][j * 2] = part[j + i * PART_IN_BLOCK].getPosition().x;
			i_vec[i][j * 2 + 1] = part[j + i * PART_IN_BLOCK].getPosition().y;
		}
	}
}


void ParticlesSystem::testParticles(GameDataRef data) {
	
	data->window.popGLStates();
	data->window.setActive(true);

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
	while (data->window.isOpen()) {
		while (data->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				data->window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))// Тригер для нажатия ЛКМ
			LB_press = 1;
		else
			LB_press = 0;

		glClear(GL_COLOR_BUFFER_BIT); // Очистка буфера

		for (auto it = 0; it < all_part.size(); ++it) {
			if (LB_press) { // Если нажата ЛКМ, то добавляем силы к частицам
				sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(data->window)); // Записываем координаты мышки относительно нашего окна
				sf::Vector2f vec1((mouse - all_part[it].getPosition()) * (G / pow(getDistance(std::move(mouse), all_part[it].getPosition()) + 5, 2)));
				sf::Vector2f vec2(-all_part[it].getSpeed() * 100.0f);
				all_part[it].addForce(vec1);
				all_part[it].addForce(vec2); // Тормозящая сила (сила сопротивления), вместе 100*f можно подставить любую магическую константу, чем больше - тем меньшее ускорение у частиц.
			}
			//Обновляем частицу
			all_part[it].update(std::move(frame_time)); // Передает в update информацию о том, сколько времени ушло для вычисления всей информации на предыдущем кадре
			if (LB_press) all_part[it].cler_force(); // Очищаем вектор скоростей

		}

		glPushMatrix();

		particleTransform(all_part, coords); // Переносим обновленные данные из вектора в массив

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
		data->window.display(); // Отображаем кадр.
		frame_time = clock.restart().asSeconds(); // Записываем прошедшее время и перезапускаем часы.

	}
	for (int i = 0; i < NUMB_OF_BLOCK; ++i) // Освобождаем динамическую память.
		delete[] coords[i];
	for (int i = 0; i < NUMB_OF_BLOCK; ++i) // Освобождаем динамическую память.
		delete[] colors[i];
}