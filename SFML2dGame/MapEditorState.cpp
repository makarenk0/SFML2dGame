#include "MapEditorState.h"

MapEditorState::MapEditorState(GameDataRef data, std::string customMapName) : _data(data), _customMapName(customMapName)
{

}

MapEditorState::MapEditorState(GameDataRef data, int mapWidth, int mapHeigth) : _data(data), _mapWidth(mapWidth), _mapHeight(mapHeigth)
{

}

MapEditorState::~MapEditorState()
{
	if (!_customMapName.empty()) {
		delete map;
	}
	delete _quickMenuBar;
}

void MapEditorState::Init() {
	
	view.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
    initToolSprites();

	if (!_customMapName.empty()) {
		map = new TileMapEditor(_data, _customMapName);
	}
	else {
		map = new TileMapEditor(_data, _mapWidth, _mapHeight);
	}
	_quickMenuBar = new QuickMenuBar(_data);

}

void MapEditorState::initToolSprites() {
	toolWidth = 300;
	toolCanvas.create(toolWidth, windowHeight*2);
	toolWindow.setPosition(0, 0);

	sf::RenderTexture selectIndicatorTexture;
	selectIndicatorTexture.create(38, 38);
	selectIndicatorTexture.clear(sf::Color(255, 255, 255));
	_selectIndicator.setTexture(selectIndicatorTexture.getTexture());

	_data->asset.loadTexture("Arrow button", ARROW_BUTTON_FILEPATH);
	_data->asset.loadTexture("Tool tiles", TOOL_TILES_FILEPATH);
	_data->asset.loadTexture("Background1", BACKGROUND1_FILEPATH);

	arrowButton.setTexture(_data->asset.getTexture("Arrow button"));
	arrowButton.setOrigin(arrowButton.getGlobalBounds().width / 2, arrowButton.getGlobalBounds().height / 2);
	arrowButton.setPosition(toolWidth + arrowButton.getGlobalBounds().width / 2, arrowButton.getGlobalBounds().height / 2);

	offset = toolWidth;

	
	int range = 20;
	int tilesInARow = 5;
	for (int i = 0; i < (_data->asset.getTexture("Tool tiles").getSize().x / tileSize * _data->asset.getTexture("Tool tiles").getSize().y / tileSize); i++) {
		sf::Sprite sprite;
		sprite.setTexture(_data->asset.getTexture("Tool tiles"));
		sprite.setTextureRect(sf::IntRect(i * tileSize % _data->asset.getTexture("Tool tiles").getSize().x, (i * tileSize / _data->asset.getTexture("Tool tiles").getSize().x) * tileSize, tileSize, tileSize));
		sprite.setPosition(i % tilesInARow * tileSize + (i % tilesInARow + 1) * range, i / tilesInARow * tileSize + (i / tilesInARow + 1) * range);
		toolSprites.push_back(sprite);
	}
	id = 0;
	_selectIndicator.setPosition(20 - 3, 20 - 3);

	redrawToolWindow();

}

void MapEditorState::HandleInput() {
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		_noHandledEvents = true;
			switch (event.type)
			{
			case sf::Event::Closed:
				_data->window.close();
				break;
			case sf::Event::MouseWheelMoved:
				if (_data->input.isMouseInRect(toolWindow.getTextureRect(), _data->window)) {
					scrollToolWindow(event.mouseWheel.delta);
				}
				else {
					this->scaleView(event.mouseWheel.delta);
				}
				
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					_data->window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));
					if (!_customMapName.empty()) {
						map->endEditingMap(_customMapName);
						_data->machine.RemoveState();
					}
					else {
						_data->machine.AddState(StateRef(new EnterMapNameState(this->_data, map)), true);
					}
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (_data->input.isSpriteClicked(toolWindow, sf::Mouse::Left, _data->window, view) && visibleTools) {
					for (int i = 0; i < toolSprites.size(); i++) {
						if (_data->input.isSpriteClicked(toolSprites[i], sf::Mouse::Left, _data->window, view, _toolWindowScroll)) {
							_selectIndicator.setPosition(toolSprites[i].getGlobalBounds().left - 3, toolSprites[i].getGlobalBounds().top - 3);
							redrawToolWindow();
							id = i;

						}
					}
				}
				else if (_data->input.isSpriteClicked(arrowButton, sf::Mouse::Left, _data->window, view)) {
					if (visibleTools) {
						toolCanvas.clear(sf::Color::Transparent);
						visibleTools = false;
					}
					else {
						toolCanvas.clear(sf::Color(54, 54, 54, 240));
						for (auto& i : toolSprites) {
							toolCanvas.draw(i);
						}
						visibleTools = true;
					}
					toolWindow.setTexture(toolCanvas.getTexture());

				}
				else if (_quickMenuBar->isPlayButtonClicked(view)) {
					if (!_customMapName.empty()) {
						goToTesting();
					}
					else {
						_toTest = true;
						resetCamera();
						_data->machine.AddState(StateRef(new EnterMapNameState(this->_data, map, &_customMapName)), false);
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _noHandledEvents && !_data->input.isMouseInRect(sf::IntRect(toolWindow.getGlobalBounds()), _data->window, view)) {
					map->placeObject(mouseX * view.getSize().x / windowWidth + (view.getCenter().x - view.getSize().x / 2), mouseY * view.getSize().y / windowHeight + (view.getCenter().y - view.getSize().y / 2), id);
				}
				_noHandledEvents = false;
				break;
			case sf::Event::MouseMoved:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					this->scrollView(sf::Vector2i(_data->input.getMousePosition(_data->window).x - mouseX, _data->input.getMousePosition(_data->window).y - mouseY));
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _noHandledEvents && !_data->input.isMouseInRect(sf::IntRect(toolWindow.getGlobalBounds()), _data->window, view)) {
					map->placeObject(mouseX * view.getSize().x / windowWidth + (view.getCenter().x - view.getSize().x / 2), mouseY * view.getSize().y / windowHeight + (view.getCenter().y - view.getSize().y / 2), id);
				}
				break;
			}
		}
	}

void MapEditorState::Update(float dt) {
	mouseX = _data->input.getMousePosition(_data->window).x;  //relative to window
	mouseY = _data->input.getMousePosition(_data->window).y;
	toolWindowUpdate();
	_quickMenuBar->update(view);
	
}

void MapEditorState::Draw(float dt) {
	_data->window.clear(sf::Color(169, 169, 169));
	
	_data->window.draw(*map);
	_data->window.draw(toolWindow);
	_data->window.draw(arrowButton);
	_data->window.draw(*_quickMenuBar);
		
	_data->window.display();
}

void MapEditorState::Pause()
{
	
}

void MapEditorState::Resume()
{
	if (_toTest) {
		goToTesting();
	}
	resetCamera();
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

	toolWindow.setPosition(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/2 + (view.getSize().y * 1.0 / windowHeight * 1.0) * 1.0 *_toolWindowScroll);
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

void MapEditorState::goToTesting()
{
	_toTest = false;
	map->endEditingMap(_customMapName);
	_data->machine.AddState(StateRef(new GameState(this->_data, _customMapName, false)), false);
}

void MapEditorState::resetCamera()
{
	view.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
	_data->window.setView(view);
}


void MapEditorState::scrollToolWindow(int deltaScale) {
	int newToolWindowScroll = _toolWindowScroll + deltaScale * _scrollSpeedCoef;
	if (newToolWindowScroll < 5 && newToolWindowScroll + (windowHeight * 1.0 / view.getSize().y * 1.0) * 1.0 *toolWindow.getGlobalBounds().height > windowHeight) {
		_toolWindowScroll = newToolWindowScroll;
	}
	
}

void MapEditorState::redrawToolWindow()
{
	toolCanvas.clear(sf::Color(54, 54, 54, 240));
	toolCanvas.draw(_selectIndicator);
	int range = 20;
	int tilesInARow = 5;
	for (auto& i : toolSprites) {
		toolCanvas.draw(i);
	}
	toolWindow.setTexture(toolCanvas.getTexture());

	toolCanvas.display();
}
