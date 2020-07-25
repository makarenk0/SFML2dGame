#pragma once
#include "Game.h"

class BaseMap : public sf::Drawable
{
public:
	BaseMap(GameDataRef data);
	void changeTile(int x, int y, int id, bool blockTiles);

protected:
	sf::Sprite mapSprite;
	sf::RenderTexture canvas;
	std::vector<std::map<std::string, sf::VertexArray>> map;
	std::deque<std::map<std::string, sf::VertexArray>*> mapObserve;
	GameDataRef _data;

	int _tileTextureWidth, _tileTextureHeight;
	int _objectTextureWidth, _objectTextureHeight;
	int _tilesInRow, _tilesInCol;

	void setTileTextureCords(sf::Vertex* vertexPtr, int x, int y);
	void reDrawCanvas();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	
	
};

