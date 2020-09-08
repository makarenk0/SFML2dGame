#include "ParticlesSystem.h"


void ParticlesSystem::particleTransform(std::vector <Particle>const& part, float**& i_vec, int numb_of_block, int part_in_block) { // Функция переписывает координаты частиц из вектора, в двумерный массив
	for (int i = 0; i < numb_of_block; ++i) {
		for (int j = 0; j < part_in_block; ++j) {
			i_vec[i][j * 2] = part[j + i * part_in_block].getPosition().x;
			i_vec[i][j * 2 + 1] = part[j + i * part_in_block].getPosition().y;
		}
	}
}

void ParticlesSystem::addParticlesEffect(ParticlesSet* p_set)
{
	_all_sets.push_back(p_set);
}

void ParticlesSystem::update(sf::RenderWindow& window, float dt)
{
	for (int i = 0; i < _all_sets.size(); i++) {
		if (_all_sets[i]->_lifeTimer.getElapsedTime().asMilliseconds() > _all_sets[i]->_timeOffset) {
			_all_sets[i]->update(dt);
			if (_all_sets[i]->_lifeTimer.getElapsedTime().asMilliseconds() > _all_sets[i]->_lifeTime) {
				ParticlesSet* buf = _all_sets[i];
				_all_sets.erase(_all_sets.begin() + i);
				delete buf;
			}
		}
	}
}

void ParticlesSystem::draw(sf::RenderWindow& window)
{
	window.setActive(true);

	
	for (auto& set : _all_sets) {
		if (set->_lifeTimer.getElapsedTime().asMilliseconds() > set->_timeOffset) {
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
	}
	window.setActive(false);
	window.resetGLStates();
}

