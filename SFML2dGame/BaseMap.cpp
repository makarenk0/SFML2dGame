#include "BaseMap.h"


BaseMap::BaseMap(GameDataRef data) : _data(data)
{
	mapSprite.setPosition(0, 0);
	_tileTextureWidth = _data->asset.getTexture("Tiles Texture").getSize().x;
	_tileTextureHeight = _data->asset.getTexture("Tiles Texture").getSize().y;
	_objectTextureWidth = _data->asset.getTexture("Objects Texture").getSize().x;
	_objectTextureHeight = _data->asset.getTexture("Objects Texture").getSize().y;
	_objectTextureWidthElements = _objectTextureWidth / tileSize;
	_objectTextureHeightElements = _objectTextureHeight / tileSize;
}


void BaseMap::changeTile(int x, int y, int id, bool blockTiles) {

	if (x > 0 && x < canvas.getSize().x && y>0 && y < canvas.getSize().y) {
		int blockNum = x / (MAP_BLOCK_WIDTH * tileSize);

		if (blockTiles) {
			sf::Vertex* vertexTile = &map[blockNum]["blockTiles"][4.0 * (((int)(y / tileSize) * MAP_BLOCK_WIDTH) + ((x % (MAP_BLOCK_WIDTH * tileSize)) / tileSize))];
			setTileTextureCords(vertexTile, id % (_tileTextureWidth / tileSize), id / (_tileTextureWidth / tileSize));
		}
		else {
			sf::Vertex* vertexObject = &map[blockNum]["blockObjects"][4.0 * (((int)(y / tileSize) * MAP_BLOCK_WIDTH) + ((x % (MAP_BLOCK_WIDTH * tileSize)) / tileSize))];
			setTileTextureCords(vertexObject, id % (_objectTextureWidth / tileSize), id / (_objectTextureWidth / tileSize));
		}

		reDrawCanvas();
	}
}

void BaseMap::setTileTextureCords(sf::Vertex* vertexPtr, int x, int y) {
	vertexPtr[0].texCoords = sf::Vector2f(x * tileSize, y * tileSize);
	vertexPtr[1].texCoords = sf::Vector2f(x * tileSize + tileSize, y * tileSize);
	vertexPtr[2].texCoords = sf::Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
	vertexPtr[3].texCoords = sf::Vector2f(x * tileSize, y * tileSize + tileSize);
}

void BaseMap::initMapBlocks(std::map<std::string, sf::VertexArray>& block) {
	block["blockTiles"] = sf::VertexArray();
	block["blockObjects"] = sf::VertexArray();
	block["blockTiles"].resize(4.0 * MAP_BLOCK_WIDTH * _tilesInCol);
	block["blockObjects"].resize(4.0 * MAP_BLOCK_WIDTH * _tilesInCol);
	block["blockTiles"].setPrimitiveType(sf::Quads);
	block["blockObjects"].setPrimitiveType(sf::Quads);
}

void BaseMap::readValue(std::string& line, std::string& value) {
	line = line.substr(value.length() + 1, line.length());
	value = line.substr(0, line.find(","));
}

void BaseMap::reDrawCanvas() {
	for (auto& i : map) {
		canvas.draw(i.at("blockTiles"), &_data->asset.getTexture("Tiles Texture"));
		canvas.draw(i.at("blockObjects"), &_data->asset.getTexture("Objects Texture"));
	}
	mapSprite.setTexture(canvas.getTexture());
}

void BaseMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	_data->window.draw(mapSprite);
}
