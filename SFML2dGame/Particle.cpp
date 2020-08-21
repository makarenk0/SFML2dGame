#include "Particle.h"

#include "Particle.h"



Particle::Particle() // Стандартный конструктор не имеет реализации, т.к. не пригодился
{
}
Particle::Particle(int m, sf::Vector2f pos) { // Конструктор инициализирует массу и начальное положение
	mass = m;
	position = pos;
	speed = sf::Vector2f(0, 0); // Начальная скорость - 0
}

void Particle::cler_force() noexcept { // Очищает std::vector от векторов силы. Т.е. убирает все силы, которые действуют на точку
	forces.clear();
}

void Particle::addForce(sf::Vector2f& n_force) noexcept { // Добавляет вектор силы в std::vector. На точку теперь воздейтсвует новая сила
	forces.push_back(n_force);
}
sf::Vector2f Particle::getMainForce() noexcept {// Подсчитывает результирующую силу. Т.к. силу можно однозначно характеризовать вектором, мы просто складываем все вектора в std::vector 
	sf::Vector2f res(0, 0);
	for (auto it = forces.begin(); it != forces.end(); ++it)
		res += *it;
	return res;
}
void Particle::update(float&& t) noexcept {
	sf::Vector2f prevPos = position; // Сохраняем текущую позицию точку
	sf::Vector2f res_force = getMainForce(); // Находим результирующую силу

	position += 0.5f * res_force * t * t / static_cast <float>(mass) + speed * t; // Обновляем позицию на текущем кадре

	// Оба варианта определения скорости корректны - но второй работает несколько быстрее
	//speed = (res_force / static_cast <float>(mass)) * t + speed; // Обновляем скорость
	speed = (position - prevPos) / t; // Обновляем скорость

}


Particle::~Particle()
{
}
