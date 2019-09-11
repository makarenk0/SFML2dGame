#include "MapEditorState.h"

MapEditorState::MapEditorState(GameDataRef data) : _data(data)
{

}

void MapEditorState::Init() {
	
	view.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
	inputField = true;

	input.setFont(_data->asset.getFont("MainMenu Font"));
	input.setPosition(50, 50);
	input.setCharacterSize(100);
    initToolSprites();

	
	
}

void MapEditorState::initToolSprites() {
	toolWidth = 300;
	toolCanvas.create(toolWidth, windowHeight);
	toolCanvas.clear(sf::Color(220, 20, 60));
	toolCanvas.display();
	toolWindow.setPosition(0, 0);

	_data->asset.loadTexture("Arrow button", ARROW_BUTTON_FILEPATH);
	_data->asset.loadTexture("Tool tiles", TOOL_TILES_FILEPATH);
	_data->asset.loadTexture("Background1", BACKGROUND1_FILEPATH);

	arrowButton.setTexture(_data->asset.getTexture("Arrow button"));
	arrowButton.setOrigin(arrowButton.getGlobalBounds().width / 2, arrowButton.getGlobalBounds().height / 2);
	arrowButton.setPosition(toolWidth + arrowButton.getGlobalBounds().width / 2, arrowButton.getGlobalBounds().height / 2);

	offset = toolWidth;

	int range = 20;
	int tilesInARow = 5;
	for (int i = 0; i < (_data->asset.getTexture("Tool tiles").getSize().x / tileSize * _data->asset.getTexture("Tool tiles").getSize().y / tileSize) ; i++) {
		sf::Sprite sprite;
		sprite.setTexture(_data->asset.getTexture("Tool tiles"));
		sprite.setTextureRect(sf::IntRect(i*tileSize%_data->asset.getTexture("Tool tiles").getSize().x, (i * tileSize / _data->asset.getTexture("Tool tiles").getSize().x)*tileSize, tileSize, tileSize));
		sprite.setPosition(i % tilesInARow * tileSize + (i % tilesInARow + 1) * range, i / tilesInARow * tileSize + (i / tilesInARow + 1) * range);
		toolSprites.push_back(sprite);
		toolCanvas.draw(sprite);
	}

	toolWindow.setTexture(toolCanvas.getTexture());
	_data->window.draw(toolWindow);
}

void MapEditorState::HandleInput() {
	sf::Event event;

	while (_data->window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			_data->window.close();
			break;
		case sf::Event::MouseWheelMoved:
			this->scaleView(event.mouseWheel.delta);
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				if (textNum == 1) {
					inputField = false;
					map = new TileMap(_data, std::stoi(mapWidthInput), std::stoi(mapHeightInput), true);
				}
				else {
					textNum++;
					mapWidthInput = mapHeightInput;
					mapHeightInput = "";
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				map->endEditingMap("customMap");
				//delete map;
				_data->machine.RemoveState();
			}
			break;
		case sf::Event::MouseMoved:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				this->scrollView(sf::Vector2i(_data->input.getMousePosition(_data->window).x - mouseX, _data->input.getMousePosition(_data->window).y - mouseY));
			}
			break;
		case sf::Event::MouseButtonPressed:
					if (_data->input.isSpriteClicked(arrowButton, sf::Mouse::Left, _data->window, view)) {
						if (visibleTools) {
							toolCanvas.clear(sf::Color::Transparent);
							visibleTools = false;
						}
						else {
							toolCanvas.clear(sf::Color(220, 20, 60));
							for (auto& i : toolSprites) {
								toolCanvas.draw(i);
							}
							visibleTools = true;
						}
						toolWindow.setTexture(toolCanvas.getTexture());
			
					}
				
			break;
		
			
				

			
		case sf::Event::TextEntered:
			mapHeightInput += event.text.unicode;
			input.setString(mapHeightInput);
			break;
		
			
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (_data->input.isSpriteClicked(toolWindow, sf::Mouse::Left, _data->window, view) && visibleTools) {
			for (int i = 0; i < toolSprites.size(); i++) {
				if (_data->input.isSpriteClicked(toolSprites[i], sf::Mouse::Left, _data->window, toolWindow, view)) {
					id = i;
				}
			}

		}
		else {
			map->placeObject(mouseX * view.getSize().x / windowWidth + (view.getCenter().x - view.getSize().x / 2), mouseY * view.getSize().y / windowHeight + (view.getCenter().y - view.getSize().y / 2), id);
		}
			}
		
		

	}
}

void MapEditorState::Update(float dt) {
	mouseX = _data->input.getMousePosition(_data->window).x;  //relative to window
	mouseY = _data->input.getMousePosition(_data->window).y;
	if (!inputField) {
		map->canvasUpdate(view.getCenter().x);
		toolWindowUpdate();

		//std::cout << view.getCenter().x << std::endl;

	}
	
}

void MapEditorState::Draw(float dt) {
	_data->window.clear();
	if (inputField) {
		_data->window.draw(input);
	}
	else {
		map->drawEditMap();
		_data->window.draw(toolWindow);
		_data->window.draw(arrowButton);
		
	}
	_data->window.display();
}


void MapEditorState::scaleView(int deltaScale) {
	view.setSize(view.getSize().x + 10 * -deltaScale, view.getSize().y + 5 * -deltaScale);
	_data->window.setView(view);
}
void MapEditorState::scrollView(sf::Vector2i deltaScroll) {
	view.move(-deltaScroll.x, -deltaScroll.y);
	_data->window.setView(view);
}

void MapEditorState::toolWindowUpdate() {

	toolWindow.setPosition(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/2);   //test
	toolWindow.setScale(view.getSize().x / windowWidth, view.getSize().y / windowHeight);
	if (visibleTools) {
		arrowButton.setPosition(view.getCenter().x - view.getSize().x / 2+toolWindow.getGlobalBounds().width+arrowButton.getGlobalBounds().width/2, view.getCenter().y - view.getSize().y / 2 + arrowButton.getGlobalBounds().height/2);
		arrowButton.setScale(-view.getSize().x / windowWidth, view.getSize().y / windowHeight);
	}
	else {
		arrowButton.setPosition(view.getCenter().x - view.getSize().x / 2 + arrowButton.getGlobalBounds().width / 2, view.getCenter().y - view.getSize().y / 2 + arrowButton.getGlobalBounds().height/2);
		arrowButton.setScale(view.getSize().x / windowWidth, view.getSize().y / windowHeight);
	}
	
}
