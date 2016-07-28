#include "guigame.hpp"
#include <stdexcept>

namespace{
  const int tile_size = 40;
  const int border_size = 2;
  const int side_bar_width = tile_size * 4;

  const sf::Color side_bar_color(182, 228, 109);
  const sf::Color open_tile_color(120, 120, 120);
  const sf::Color closed_tile_color(50, 50, 50);
  const sf::Color mine_color(255, 81, 89);
  const sf::Color flag_color(255, 205, 70);
  const sf::Color number_colors[9] = {
    sf::Color(0, 0, 0),       // 0
    sf::Color(89, 124, 255),  // 1
    sf::Color(182, 228, 109), // 2
    sf::Color(255, 81, 89),   // 3
    sf::Color(255, 205, 70),  // 4
    sf::Color(89, 124, 255),  // 5
    sf::Color(182, 228, 109), // 6
    sf::Color(255, 81, 89),   // 7
    sf::Color(255, 205, 70)   // 8
  };
}

GuiGame::GuiGame(int height, int width, int mines) : height(height), width(width), mines(mines), flagsPlaced(0), state(Gamestate::Pending), mode(Playermode::Singleplayer){
  field = Field(height, width, mines);
  if(!font.loadFromFile("sansation.ttf")){
    throw std::runtime_error("Could not load sansation.ttf");
  }
  window = new sf::RenderWindow(sf::VideoMode(width * tile_size + border_size + side_bar_width, height * tile_size + border_size),
  "This is a nice window", sf::Style::Titlebar);
}

GuiGame::~GuiGame(){
  delete window;
  window = nullptr;
}

bool GuiGame::isOpen() const{
  return window->isOpen();
}

void GuiGame::getEvent(){
  sf::Event event;
  while(window->pollEvent(event)){
    switch(event.type){
      case sf::Event::EventType::MouseButtonPressed:
        clickAt(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
        //changed = true; // For operator <<
        break;
      case sf::Event::EventType::KeyPressed:
        switch(event.key.code){
          case sf::Keyboard::Key::Q:
            window->close();
        }
    }
  }
}

void GuiGame::updateGameState(){
  if(field.hasMinesBeenPlaced() == false){
    state = Gamestate::Pending;
  }
  else{
    if(field.hasMinesBeenOpened()){
      state = Gamestate::Lost;
    }
    else if(field.onlyMinesLeft()){
      state = Gamestate::Won;
    }
    else{
      state = Gamestate::Playing;
    }
  }
}

Gamestate GuiGame::getState() const {
  return state;
}

void GuiGame::display(){
  window->clear();
  // Start drawing tiles
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      int tileX = tile_size * x + border_size;
      int tileY = tile_size * (height - y - 1) + border_size;
      sf::RectangleShape tile;
      tile.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
      tile.setFillColor(field.isOpen(x, y) ? open_tile_color : closed_tile_color);
      tile.setPosition(tileX, tileY);
      window->draw(tile);
    }
  }
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      drawLabel(x, y);
    }
  }
  // Stop drawing tiles

  // Start drawing sidebar
  {
    sf::RectangleShape sidepane;
    sidepane.setSize(sf::Vector2f(side_bar_width - border_size, height * tile_size - border_size));
    sidepane.setFillColor(closed_tile_color);
    sidepane.setPosition(width * tile_size + border_size, border_size);
    window->draw(sidepane);

    sf::RectangleShape separator;
    separator.setSize(sf::Vector2f(side_bar_width - 10 * border_size, border_size / 2));
    separator.setFillColor(side_bar_color);
    separator.setPosition(width * tile_size + 5 * border_size, border_size);
    int distance = height * tile_size / 3;
    separator.move(0, distance);
    window->draw(separator);
    separator.move(0, distance);
    window->draw(separator);
  }
  // Start drawing indicator
  {
    std::string indicatorStr;
    switch(state){
      case Gamestate::Won:
        indicatorStr = ": ";
        break;
      case Gamestate::Lost:
        indicatorStr = ":(";
        break;
      case Gamestate::Playing:
        indicatorStr = ":)";
        break;
      case Gamestate::Pending:
        indicatorStr = ":|";
        break;
    }
    sf::Text stateLabel;
    stateLabel.setFont(font);
    stateLabel.setCharacterSize(height * tile_size / 4);
    stateLabel.setStyle(sf::Text::Bold);
    stateLabel.setString(indicatorStr);
    stateLabel.setColor(side_bar_color); // Change with state?
    sf::FloatRect boundingBox = stateLabel.getLocalBounds();
    stateLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    stateLabel.setPosition(width * tile_size + side_bar_width / 2, height * tile_size / 6);
    stateLabel.setRotation(90);
    window->draw(stateLabel);
    if(state == Gamestate::Won){
      sf::Vector2f position = stateLabel.getPosition();
      stateLabel.setString("  D");
      stateLabel.setCharacterSize(height * tile_size / 6);
      boundingBox = stateLabel.getLocalBounds();
      stateLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
      stateLabel.setPosition(position);
      stateLabel.scale(0.8, 1.5);
      window->draw(stateLabel);
    }
  }
  // Start drawing time
  {
    std::string secString, minString;
    if(state != Gamestate::Pending){
      int seconds = field.getSecondsSinceStart();
      int minutes = seconds / 60;
      seconds %= 60;
      if(minutes < 10){
        minString.push_back('0');
      }
      minString += std::to_string(minutes);
      if(seconds < 10){
        secString.push_back('0');
      }
      secString += std::to_string(seconds);
      if(minutes > 99){
        minString = "99";
        secString = "++";
      }
    }
    else{
      secString = "00";
      minString = "00";
    }
    sf::Text timeLabel;
    timeLabel.setFont(font);
    timeLabel.setCharacterSize(height * tile_size / 16);
    timeLabel.setStyle(sf::Text::Bold);
    timeLabel.setString(minString + ":" + secString); ///
    timeLabel.setColor(side_bar_color); ///
    sf::FloatRect boundingBox = timeLabel.getLocalBounds();
    timeLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    timeLabel.setPosition(width * tile_size + side_bar_width / 2, height * tile_size / 2);
    window->draw(timeLabel);
  }
  // Start drawing minecounter
  {
    sf::Text mineLabel;
    mineLabel.setFont(font);
    mineLabel.setCharacterSize(height * tile_size / 4);
    mineLabel.setStyle(sf::Text::Bold);
    if(state != Gamestate::Playing){
      mineLabel.setString(std::to_string(mines));
    }
    else{
      mineLabel.setString(std::to_string(mines - flagsPlaced));
    }
    mineLabel.setColor(side_bar_color);
    sf::FloatRect boundingBox = mineLabel.getLocalBounds();
    mineLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    mineLabel.setPosition(width * tile_size + side_bar_width / 2, height * tile_size * 5 / 6);
    window->draw(mineLabel);

    // Mines or Flags
    int labelOffset = height * tile_size * 5 / 6 + boundingBox.height * 2 / 3;
    sf::Text minesOrFlags;
    minesOrFlags.setFont(font);
    minesOrFlags.setCharacterSize(height * tile_size / 24);
    if(state != Gamestate::Playing){
      minesOrFlags.setString("mines");
    }
    else{
      minesOrFlags.setString("left");
    }
    minesOrFlags.setColor(side_bar_color);
    boundingBox = minesOrFlags.getLocalBounds();
    minesOrFlags.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top);
    minesOrFlags.setPosition(width * tile_size + side_bar_width / 2, labelOffset);
    window->draw(minesOrFlags);
  }
  // Stop drawing sidebar
  window->display();
}

void GuiGame::updateTitle(){
  updateGameState();
  std::string title;
  switch(state){
    case Gamestate::Won:
      title = "Won";
      break;
    case Gamestate::Lost:
      title = "Lost";
      break;
    case Gamestate::Playing:
      title = "Playing";
      break;
    case Gamestate::Pending:
      title = "Pending";
      break;
  }
  window->setTitle(title);
}

void GuiGame::clickAt(int x, int y, sf::Mouse::Button button){
  if(x < width * tile_size + border_size){ // Clicked on tile field
    if(state != Gamestate::Won && state != Gamestate::Lost){
      x -= border_size;
      y -= border_size;
      y /= tile_size;
      y = height - y - 1;
      x /= tile_size;
      if(button == sf::Mouse::Button::Left){ // Open
        if(field.isFlagged(x, y) == false){
          field.setOpen(x, y);
        }
      }
      else if(button == sf::Mouse::Button::Right){ // Flag
        if(field.isFlagged(x, y)){
          flagsPlaced--;
          field.toggleFlag(x, y, "Babbage");
        }
        else{
          if(flagsPlaced < mines && !field.isOpen(x, y)){
            flagsPlaced++;
            field.toggleFlag(x, y, "Babbage");
          }
        }
      }
    }
  }
  else{ // Clicked on sidebar
    if(y <= height * tile_size / 3){ // Clicked on indicator field
      state = Gamestate::Pending;
      flagsPlaced = 0;
      field = Field(height, width, mines);
    }
    else if(y > height * tile_size * 2 / 3){ // Clicked on minecounter
      if(state != Gamestate::Playing){ // Allow changing mines
        if(button == sf::Mouse::Button::Left){
          mines++;
        }
        else{
          mines--;
        }
      }
      if(state == Gamestate::Pending){
        field = Field(height, width, mines);
      }
    }
  }
}

void GuiGame::drawLabel(int x, int y){
  sf::Text label;
  label.setFont(font);
  label.setCharacterSize(tile_size / 2);
  label.setStyle(sf::Text::Bold);
  bool needToDraw = true;
  if(field.isOpen(x, y)){              // tile open
    if(field.isMine(x, y)){
      label.setString("x");
      label.setColor(mine_color);
    }
    else{
      int mines = field.getSurroundingMines(x, y);
      label.setString(std::to_string(mines));
      label.setColor(number_colors[mines]);
      if(mines == 0){
        needToDraw = false;
      }
    }
  }
  else{                               // tile closed
    if(field.isFlagged(x, y)){
      label.setString("F");
      label.setColor(flag_color);
    }
    else{
      needToDraw = false;
    }
  }
  sf::FloatRect labBox = label.getLocalBounds();
  label.setOrigin(labBox.left + labBox.width / 2, labBox.top + labBox.height / 2);
  label.setPosition(x * tile_size + tile_size / 2 + border_size, tile_size * (height - y - 1) + tile_size / 2);
  if(needToDraw){
    window->draw(label);
  }
}

/*
std::ostream& operator << (std::ostream& stream, GuiGame& object){
  if(object.changed){
    stream << object.field << std::endl;
    object.changed = false;
  }
  return stream;
}
*/
