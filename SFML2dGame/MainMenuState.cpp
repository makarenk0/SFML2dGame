#include "MainMenuState.h"
#include "MapEditorState.h"
#include "GameState.h"

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{

}

void MainMenuState::Init() {
	
	_data->asset.loadTexture("MainMenu Background", MAINMENU_STATE_BACKGROUND_FILEPATH);
	_background.setTexture(_data->asset.getTexture("MainMenu Background"));

	_data->asset.loadFont("MainMenu Font", MAINMENU_STATE_FONT_FILEPATH);
	_data->asset.loadTexture("Tiles Texture", TILES_TEXTURE_FILEPATH);
	_data->asset.loadTexture("Objects Texture", OBJECTS_TEXTURE_FILEPATH);

	_data->asset.loadTexture("Background1", BACKGROUND1_TEXTURE_FILEPATH);
	_data->asset.loadTexture("Background2", BACKGROUND2_TEXTURE_FILEPATH);
	_data->asset.loadTexture("Clouds", CLOUDS_TEXTURE_FILEPATH);

	_data->asset.loadTexture("InfoBar", INFOBAR_TEXTURE_FILEPATH);
	_data->asset.loadTexture("Heart", HEART_TEXTURE_FILEPATH);

	_data->asset.loadSound("Hit player", HIT_PLAYER_SOUND_FILEPATH);
	_data->asset.loadSound("Menu Music", MENU_MUSIC_FILEPATH);
	_data->asset.loadSound("Gameover Sound", GAMEOVER_MUSIC_FILEPATH);
	_data->asset.loadSound("Victory Sound", VICTORY_MUSIC_FILEPATH);
	
	
	int rangeBetweenButtons = 30;
	_singlePlayerText.setFont(_data->asset.getFont("MainMenu Font"));
	_singlePlayerText.setString("singleplayer");
	_singlePlayerText.setCharacterSize(100);
	_singlePlayerText.setPosition(sf::Vector2f(windowWidth/2 - _singlePlayerText.getGlobalBounds().width/2, windowHeight/3));


	_mapEditorText.setFont(_data->asset.getFont("MainMenu Font"));
	_mapEditorText.setString("map editor");
	_mapEditorText.setCharacterSize(100);
	_mapEditorText.setPosition(sf::Vector2f(windowWidth / 2 - _singlePlayerText.getGlobalBounds().width / 2, windowHeight / 3+ _mapEditorText.getGlobalBounds().height+rangeBetweenButtons));

	menuCanvas.create(windowWidth, windowHeight);
	menuCanvas.draw(_background);
	menuCanvas.draw(_singlePlayerText);
	menuCanvas.draw(_mapEditorText);
	menuCanvas.display();

	menuSprite.setTexture(menuCanvas.getTexture());
	menuSprite.setPosition(0, 0);

	menuView.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
	
	
	menuMusic.setBuffer(_data->asset.getSound("Menu Music"));
	menuMusic.setLoop(true);
	menuMusic.play();
}

void MainMenuState::HandleInput() {
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			_data->window.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (_data->input.isTitleClicked(_singlePlayerText, sf::Mouse::Button::Left, _data->window)) {
				_data->machine.AddState(StateRef(new GameState(this->_data)), false);
			}
			else if (_data->input.isTitleClicked(_mapEditorText, sf::Mouse::Button::Left, _data->window)) {
				_data->machine.AddState(StateRef(new MapEditorState(this->_data)), false);
			}
			break;

		}
	}
}

void MainMenuState::Update(float dt) {
}

void MainMenuState::Draw(float dt) {
	_data->window.clear();
	_data->window.draw(menuSprite);
	_data->window.display();
}

void MainMenuState::Pause() {
	menuMusic.stop();
}

void MainMenuState::Resume() {
	_data->window.setView(menuView);
	menuMusic.play();
}
