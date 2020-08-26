#include "EnterMapNameState.h"

EnterMapNameState::EnterMapNameState(GameDataRef data, TileMapEditor* map) : _data(data), _map(map)
{

}

EnterMapNameState::EnterMapNameState(GameDataRef data, TileMapEditor* map, std::string* customMapName) : _data(data), _map(map), _mapNamePtr(customMapName)
{
}

EnterMapNameState::~EnterMapNameState()
{
	delete _map;
	delete _backgroundMap;
}

void EnterMapNameState::Init()
{
	_backgroundMap = new TileMapEditor(_data, windowWidth, windowHeight);
	srand(time(NULL));
	_mapNameDisplay = sf::Text();
	_mapNameDisplay.setFont(_data->asset.getFont("MainMenu Font"));
	_mapNameDisplay.setFillColor(sf::Color(255, 255, 255));
	_mapNameDisplay.setCharacterSize(100);
	_mapNameDisplay.setPosition(windowWidth / 3, windowHeight / 3 + 50);

	_explanationText = sf::Text();
	_explanationText.setFont(_data->asset.getFont("MainMenu Font"));
	_explanationText.setFillColor(sf::Color(255, 255, 255));
	_explanationText.setCharacterSize(100);
	_explanationText.setPosition(370, windowHeight / 3 - 100);
	_explanationText.setString("Enter map name:");
}

void EnterMapNameState::HandleInput()
{
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			_data->window.close();
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !_mapName.empty()) {
				_mapName.pop_back();
				_mapNameDisplay.setString(_mapName);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				_map->endEditingMap(_mapName);
				if (_mapNamePtr != nullptr) {
					*_mapNamePtr = _mapName;
				}
				_data->machine.RemoveState();
			}
			break;
		case sf::Event::TextEntered:
			if (event.text.unicode != 8) {
				_mapName += event.text.unicode;
				_mapNameDisplay.setString(_mapName);
			}
			break;
		}
	}
}

void EnterMapNameState::Update(float dt)
{
	if (triggerLife.getElapsedTime().asMilliseconds() > 100) {
		int x = rand() % windowWidth;
		int y = rand() % windowHeight;
		int id = rand() % 89;
		_backgroundMap->placeObject(x, y, id);
		triggerLife.restart();
	}
}

void EnterMapNameState::Draw(float dt)
{
	_data->window.clear();
	_data->window.draw(*_backgroundMap);
	_data->window.draw(_explanationText);
	_data->window.draw(_mapNameDisplay);
	_data->window.display();
}
