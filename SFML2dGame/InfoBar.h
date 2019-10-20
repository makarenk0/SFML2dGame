#pragma once
#include "Game.h"
#include <iostream>
class InfoBar
{
public:
	InfoBar(int x, int y, int width, int heigth, GameDataRef data, sf::Vector2f cameraSize, int startHealth);
	void updateInfoBar(int x, int y);
	void drawInfoBar();
	bool changeHealth(int delta);
	void setHealth(int health);
private:
	sf::Sprite _background;
	int _width, _height;
	GameDataRef _data;
	sf::RenderTexture canvasInfo;
	sf::Sprite spriteInfo;
	sf::Texture heartTexture;
	struct Health {
		std::vector<std::pair<sf::Sprite, sf::Texture*>> health;
		sf::Sound playerHitSound;

		
		void setHealth(int healthCount, sf::Texture heartTexture) {
			for(auto &i : health) {
				delete i.second;
				health.pop_back();
			}
			if (healthCount > 5) {
				healthCount = 5;
			}
			for (int i = 0; i < healthCount; i++) {
				sf::Texture* bufTexture = new sf::Texture(heartTexture);
				sf::Sprite heart(*bufTexture);
				heart.setPosition(i * 15 + i * 30 + 15, 50);
				health.push_back(std::pair<sf::Sprite, sf::Texture*>(heart, bufTexture));
			}
		}
		bool deltaHealth(int deltaHealth, sf::Texture heartTexture) {
			int hSize = health.size();
			if (deltaHealth+ hSize <= 0) {
				playerHitSound.play();
				return true;
			}
			else if (health.size() + deltaHealth > 5) {
				setHealth(5, heartTexture);
			}
			else {
				if (deltaHealth > 0) {
					for (int i = 0; i < health.size() + deltaHealth; i++) {
						sf::Texture* bufTexture = new sf::Texture(heartTexture);
						sf::Sprite heart(*bufTexture);
						heart.setPosition(i * 15 + i * 30 + 15, 50);
						health.push_back(std::pair<sf::Sprite, sf::Texture*>(heart, bufTexture));
					}
				}
				else {
					playerHitSound.play();
					int size = health.size()-1;
					for (int i = 0; i < -deltaHealth; i++) {
						delete health[size - i * 1.0].second;
						health.pop_back();
					}
				}
			}
			return false;
		}

		void setPlayerHitSound(sf::SoundBuffer& sound) {
			playerHitSound.setBuffer(sound);
		}

		void draw(sf::RenderTexture& canvas) {
			for (auto& i : health) {
				canvas.draw(i.first);
			}
			
		}
	};
	Health playerHealth;
	void reDrawCanvas();

	
	
};
