#include "MapEditorState.h"

MapEditorState::MapEditorState(GameDataRef data) : _data(data)
{

}

void MapEditorState::Init() {
	
	view.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
	inputField = true;

	inputWidth.setFont(_data->asset.getFont("MainMenu Font"));
	inputWidth.setPosition(windowWidth/2-100, windowHeight/4);
	inputWidth.setCharacterSize(100);

	inputHeight.setFont(_data->asset.getFont("MainMenu Font"));
	inputHeight.setPosition(windowWidth/2-100, windowHeight/2-25);
	inputHeight.setCharacterSize(100);


    explanationText.setFont(_data->asset.getFont("MainMenu Font"));
	explanationText.setString("Map width:\n\nMap height:");
    explanationText.setPosition(windowWidth / 5, windowHeight / 4);
    explanationText.setCharacterSize(100);


	inputName.setFont(_data->asset.getFont("MainMenu Font"));
	inputName.setPosition(windowWidth / 2 - 100, windowHeight / 4);
	inputName.setCharacterSize(100);
	
	
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
		if (inputField) {
			switch (event.type) {
				case sf::Event::KeyPressed:
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						switch (inputSizeState) {
						case 0:
							mapWidthInput = std::stoi(inputWidthText);
							inputSizeState++;
							break;
						case 1:
							inputField = false;
							mapHeightInput = std::stoi(inputHeightText);
							map = new TileMap(_data, mapWidthInput, mapHeightInput, true);
							break;
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
						switch (inputSizeState) {
						case 0:
							if (inputWidthText.length() != 0) {
								inputWidthText.pop_back();
								inputWidth.setString(inputWidthText);
							}
							break;
						case 1:
							if (inputHeightText.length() == 0) {
								inputWidthText.pop_back();
								inputWidth.setString(inputWidthText);
								inputSizeState--;
							}
							else {
								inputHeightText.pop_back();
								inputHeight.setString(inputHeightText);
							}
							break;
						}
					}
					break;
				case sf::Event::TextEntered:
					if (event.text.unicode != 8) {
						switch (inputSizeState) {
						case 0:
							inputWidthText += event.text.unicode;
							inputWidth.setString(inputWidthText);
							break;
						case 1:
							inputHeightText += event.text.unicode;
							inputHeight.setString(inputHeightText);
							break;
						}
					}
					break;
			}
		}
		else if (endMap) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						if (mapName.length() == 0) {
							map->endEditingMap("customMap");
						}
						else {
							map->endEditingMap(mapName);
						}
						_data->machine.RemoveState();
				}
				break;
			case sf::Event::TextEntered:
				if (event.text.unicode != 27) {
					mapName += event.text.unicode;
					inputName.setString(mapName);
				}
			break;
			}
		}
		else {
			switch (event.type)
			{
			case sf::Event::Closed:
				_data->window.close();
				break;
			case sf::Event::MouseWheelMoved:
				this->scaleView(event.mouseWheel.delta);
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					_data->window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));
					endMap = true;
					explanationText.setString("Map name:");
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
			}
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
	}
	
}

void MapEditorState::Draw(float dt) {
	_data->window.clear();
	if (inputField) {
		_data->window.draw(inputWidth);
		_data->window.draw(inputHeight);
		_data->window.draw(explanationText);
	}
	else if (endMap) {
		_data->window.draw(inputName);
		_data->window.draw(explanationText);
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
