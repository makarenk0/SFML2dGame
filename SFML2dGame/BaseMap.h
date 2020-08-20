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
	GameDataRef _data;

	int _tileTextureWidth, _tileTextureHeight;
	int _objectTextureWidth, _objectTextureHeight;
	int _objectTextureWidthElements, _objectTextureHeightElements;
	int _tilesInRow, _tilesInCol;

	void readValue(std::string& line, std::string& value);
	void setTileTextureCords(sf::Vertex* vertexPtr, int x, int y);
	void initMapBlocks(std::map<std::string, sf::VertexArray>& block);
	
	virtual void initMap(std::string mapName, bool campaignMode) = 0;
	virtual void reDrawCanvas();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	
	
};

