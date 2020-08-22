#include "ParticlesSet.h"


ParticlesSet::ParticlesSet(int x, int y, int numb_of_block, int part_in_block, float lifeTime) : _x(x), _y(y), _numb_of_block(numb_of_block), _part_in_block(part_in_block), _lifeTime(lifeTime)
{
	coords = new float* [_numb_of_block];
	for (int i = 0; i < _numb_of_block; ++i)
		coords[i] = new float[_part_in_block * 2];

	colors = new unsigned char* [_numb_of_block];
	for (int i = 0; i < _numb_of_block; ++i)
		colors[i] = new unsigned char[_part_in_block * 3];
}
ParticlesSet::~ParticlesSet() {
	for (int i = 0; i < _numb_of_block; ++i)
		delete[] coords[i];
	for (int i = 0; i < _numb_of_block; ++i)
		delete[] colors[i];
}
	
float ParticlesSet::getDistance(sf::Vector2f&& v1, sf::Vector2f&& v2) noexcept {
	return sqrt(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2));
}