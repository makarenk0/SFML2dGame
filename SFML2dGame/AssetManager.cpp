#include "AssetManager.h"

void AssetManager::loadTexture(std::string name, std::string fileName)
{
	sf::Texture tex;
	if (tex.loadFromFile(fileName)) {
		this->_textures[name] = tex;
	}
}

sf::Texture& AssetManager::getTexture(std::string name)
{
	return this->_textures.at(name);
}

void AssetManager::loadFont(std::string name, std::string fileName)
{
	sf::Font font;
	if (font.loadFromFile(fileName)) {
		this->_fonts[name] = font;
	}
}

sf::Font& AssetManager::getFont(std::string name)
{
	return this->_fonts.at(name);
}

void AssetManager::loadSound(std::string name, std::string fileName)
{
	sf::SoundBuffer sound;
	if (sound.loadFromFile(fileName)) {
		this->_sounds[name] = sound;
	}
}

sf::SoundBuffer& AssetManager::getSound(std::string name)
{
	return this->_sounds.at(name);
}
