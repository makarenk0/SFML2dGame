#include "MainMenuState.h"
#include "MapEditorState.h"
#include "GameState.h"
#include "MenuItem.h"


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

    _data->asset.loadTexture("Inventory Slot", INVENTORY_SLOT_TEXTURE_FILEPATH);

    _data->asset.loadSound("Hit player", HIT_PLAYER_SOUND_FILEPATH);
    _data->asset.loadSound("Menu Music", MENU_MUSIC_FILEPATH);
    _data->asset.loadSound("Gameover Sound", GAMEOVER_MUSIC_FILEPATH);
    _data->asset.loadSound("Victory Sound", VICTORY_MUSIC_FILEPATH);

    _data->asset.loadTexture("Play Button", PLAY_BUTTON_FILEPATH);
    
    
    
    

    #pragma region InitMenuList
    rangeBetweenButtons = 30;


    initMenuSection("main_selection", std::vector<std::string>({"Singleplayer", "Map editor"}));

    initMenuSection("singleplayer", std::vector<std::string>({ "Campaign", "Custom map" }));

    initMenuSection("map_editor", std::vector<std::string>({ "Edit map", "New map" }));

    loadMapsLists();
    loadSizesVariants();

    currentMenuList = menuListData["main_selection"];
    #pragma endregion


    menuCanvas.create(windowWidth, windowHeight);
    menuSprite.setPosition(0, 0);

    menuView.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
    
    menuMusic.setBuffer(_data->asset.getSound("Menu Music"));
    menuMusic.setLoop(true);
    menuMusic.play();

    _data->particles.addParticlesEffect(new BloodParticlesSet(100, 100, 500, _data->window));

    _parallelParticles = std::thread(&MainMenuState::particlesUpdateParallelThread, this);


}

void MainMenuState::HandleInput() {
    sf::Event event;
    while (_data->window.pollEvent(event)) {
        switch (event.type)
        {
        case sf::Event::Closed:
            menuMusic.stop();
            _data->window.close();
            break;
        case sf::Event::MouseButtonPressed:
            menuNavigate();
            computeMenuListLength();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                switch (_state) {
                case 1:
                case 4:
                    currentMenuList = menuListData["main_selection"];
                    _state = 0;
                    break;
                case 2:
                case 3:
                    currentMenuList = menuListData["singleplayer"];
                    _state = 1;
                    break;
                case 5:
                case 6:
                    currentMenuList = menuListData["map_editor"];
                    _state = 4;
                    break;
                
                }
            }
            computeMenuListLength();
            currentScrollState = 0;
            break;
        case sf::Event::MouseWheelMoved:
            int wheelDelta = event.mouseWheel.delta;
            if (currentScrollState - (wheelDelta * scrollSpeed) < 0
                && currentScrollState + currentMenuListLength - currentMenuList.back().getRect().height - (wheelDelta * scrollSpeed) > 0
                && currentMenuList.size()>4) {
                currentScrollState -= (wheelDelta * scrollSpeed);
                scrollMenuListItems(wheelDelta);
            }
            break;
        }
        
    }
}

void MainMenuState::Update(float dt) {
   
    for (auto& i : currentMenuList) {
        if (_data->input.isMouseInRect(i.getRect(), _data->window)) {
            i.setVisible();
        }
        else {
            i.setTransparent();
        }
    }
    
    
    
}

void MainMenuState::Draw(float dt) {
    _dt = dt;
    _data->window.clear();
   
    menuCanvasRedraw();
    _data->window.draw(menuSprite);
   
    
    _data->particles.draw(_data->window);
    _data->window.display();
}

void MainMenuState::Pause() {
    menuMusic.stop();
}

void MainMenuState::Resume() {
    _data->window.setView(menuView);
    menuMusic.play();
    currentScrollState = 0;
    loadMapsLists();
    computeMenuListLength();
}

void MainMenuState::menuCanvasRedraw()
{
    menuCanvas.clear();
    menuCanvas.draw(_background);
    
    for (auto& i : currentMenuList) {
        menuCanvas.draw(i);
    }
    menuSprite.setTexture(menuCanvas.getTexture());
    menuCanvas.display();
}

void MainMenuState::menuNavigate()
{
    switch (_state) {
    case 0:
        if (_data->input.isRectClicked(currentMenuList.front().getRect(), sf::Mouse::Button::Left, _data->window)) {
            currentMenuList = menuListData["singleplayer"];
            _state = 1;
        }
        else if (_data->input.isRectClicked(currentMenuList.back().getRect(), sf::Mouse::Button::Left, _data->window)) {
            currentMenuList = menuListData["map_editor"];
            _state = 4; 
        }
        break;
    case 1:
        if (_data->input.isRectClicked(currentMenuList.front().getRect(), sf::Mouse::Button::Left, _data->window)) {
            currentMenuList = menuListData["campaign"];
            _state = 2;
        }
        else if (_data->input.isRectClicked(currentMenuList.back().getRect(), sf::Mouse::Button::Left, _data->window)) {
            currentMenuList = menuListData["custom"];
            _state = 3;
        }
        break;
    case 2:
        for (auto& i : currentMenuList) {
            if (_data->input.isRectClicked(i.getRect(), sf::Mouse::Button::Left, _data->window)) {
                _data->machine.AddState(StateRef(new GameState(this->_data, i.getText(), true)), false);
                currentMenuList = menuListData["campaign"];
            }
        }
        break;
    case 3:
        for (auto& i : currentMenuList) {
            if (_data->input.isRectClicked(i.getRect(), sf::Mouse::Button::Left, _data->window)) {        
                _data->machine.AddState(StateRef(new GameState(this->_data, i.getText(), false)), false);
                currentMenuList = menuListData["custom"];
            }
        }
        break;
    case 4:
        if (_data->input.isRectClicked(currentMenuList.front().getRect(), sf::Mouse::Button::Left, _data->window)) {
            currentMenuList = menuListData["custom"];
            _state = 5;
        }
        else if (_data->input.isRectClicked(currentMenuList.back().getRect(), sf::Mouse::Button::Left, _data->window)) {
            currentMenuList = menuListData["map_sizes_select"];
            _state = 6;
        }
        break;
    case 5:
        for (auto& i : currentMenuList) {
            if (_data->input.isRectClicked(i.getRect(), sf::Mouse::Button::Left, _data->window)) {
                _data->machine.AddState(StateRef(new MapEditorState(this->_data, i.getText())), false);
                currentMenuList = menuListData["custom"];
            }
        }
        break;
    case 6:
        for (auto& i : currentMenuList) {
            if (_data->input.isRectClicked(i.getRect(), sf::Mouse::Button::Left, _data->window)) {
                std::string size = i.getText();
                _data->machine.AddState(StateRef(new MapEditorState(this->_data, std::stoi(size.substr(0, size.find(" "))), std::stoi(size.substr(size.find_last_of(" "))))), false);
            }
        }
        break;

    }
}

void MainMenuState::computeMenuListLength()
{
    currentMenuListLength = 0;
    for (auto& i : currentMenuList) {
        currentMenuListLength += i.getRect().height + rangeBetweenButtons;
    }
}

void MainMenuState::scrollMenuListItems(const int& wheelDelta)
{
    for (auto& i : currentMenuList) {
        i.setTransform(0, wheelDelta * scrollSpeed);
    }
}

void MainMenuState::loadMapsLists()
{
    std::string type = "campaign";
    for (int i = 0; i < 2; i++) {
        menuListData[type].clear();
        menuListData.insert({ type, std::vector<MenuItem>() });
        int j = 0;
        for (const auto& entry : fs::directory_iterator("maps/" + type + "/")) {
            std::string fullName = entry.path().filename().string();
            menuListData[type].push_back(MenuItem(fullName.substr(0, fullName.length() - 4), _data->asset.getFont("MainMenu Font"), 100, sf::Color(255, 255, 255), sf::Color(0, 0, 255)));
            menuListData[type].back().setPosition(windowWidth / 2 - menuListData[type].back().getRect().width / 2, 150 + j * 130 + j * rangeBetweenButtons);
            ++j;
        }
        type = "custom";
    }
}

void MainMenuState::loadSizesVariants()
{
    std::vector<std::string> sizes;
    sizes.push_back("1600 x 800");
    sizes.push_back("3200 x 800");
    sizes.push_back("3200 x 1600");
    sizes.push_back("4800 x 1600");
    sizes.push_back("4800 x 3200");
    sizes.push_back("1600 x 1600");
    sizes.push_back("3200 x 3200");
    sizes.push_back("4800 x 4800");
    initMenuSection("map_sizes_select", sizes);
}

void MainMenuState::initMenuSection(const std::string& menuSectionName, std::vector<std::string> items)
{
    menuListData.insert({ menuSectionName, std::vector<MenuItem>() });

    menuListData[menuSectionName].push_back(MenuItem(*items.begin(), _data->asset.getFont("MainMenu Font"), 100, sf::Color(255, 255, 255), sf::Color(0, 0, 255)));
    menuListData[menuSectionName].back().setPosition(windowWidth / 2 - menuListData[menuSectionName].back().getRect().width / 2, windowHeight / 3);
    
    if (items.size() > 1) {
        int j = 1;
        for (std::vector<std::string>::iterator i = items.begin() + 1; i != items.end(); i++, j++) {
            menuListData[menuSectionName].push_back(MenuItem(*i, _data->asset.getFont("MainMenu Font"), 100, sf::Color(255, 255, 255), sf::Color(0, 0, 255)));
            menuListData[menuSectionName].back().setPosition(windowWidth / 2 - menuListData[menuSectionName].back().getRect().width / 2, windowHeight / 3 + j*menuListData[menuSectionName].front().getRect().height + j*rangeBetweenButtons);
        }
    }

}

void MainMenuState::particlesUpdateParallelThread()
{
    while (true) {
        _data->particles.update(_data->window, _dt);
    }
}
