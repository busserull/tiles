#include "guigame.hpp"
#include <stdexcept>

#include <chrono>
#include <thread>
#include <cstdlib>
#include <iostream>

namespace{
  const int tile_size = 40;
  const int border_size = 2;

  const sf::Color open_tile_color(120, 120, 120);
  const sf::Color closed_tile_color(50, 50, 50);
  const sf::Color mine_color(255, 81, 89);
  const sf::Color flag_color(255, 205, 70);
  const sf::Color number_colors[9] = {
    sf::Color(0, 0, 0),       // x
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

GuiGame::GuiGame(int height, int width, int mines) : height(height), width(width), state(Gamestate::Pending){
  field = Field(height, width, mines);
  if(!font.loadFromFile("sansation.ttf")){
    throw std::runtime_error("Could not load sansation.ttf");
  }
  window = new sf::RenderWindow(sf::VideoMode(width * tile_size + border_size, height * tile_size + border_size),
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
        changed = true; ///
        break;
      case sf::Event::EventType::KeyPressed:
        switch(event.key.code){
          case sf::Keyboard::Key::Q:
            window->close();
        }
    }
  }
}

void GuiGame::display(){
  window->clear();
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
  window->display();
}

Gamestate GuiGame::getState(){
  return Gamestate::Pending;
}

void GuiGame::clickAt(int x, int y, sf::Mouse::Button button){
  x -= border_size;
  y -= border_size;
  y /= tile_size;
  y = height - y - 1;
  x /= tile_size;
  if(button == sf::Mouse::Button::Left){
    field.toggleOpen(x, y);
    if(state == Gamestate::Pending){
      field.placeMines(x, y);
      state = Gamestate::Playing;
    }
  }
  else if(button == sf::Mouse::Button::Right){
    field.toggleFlag(x, y, "Babbage");
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

    }
  }
  else{                               // tile closed
    if(field.isFlagged(x, y)){
      label.setString("F");
      label.setColor(flag_color);
    }
  }
  sf::FloatRect labBox = label.getLocalBounds();
  label.setOrigin(labBox.left + labBox.width / 2, labBox.top + labBox.height / 2);
  label.setPosition(x * tile_size + tile_size / 2 + border_size, tile_size * (height - y - 1) + tile_size / 2);
  if(needToDraw){
    window->draw(label);
  }
}

std::ostream& operator << (std::ostream& stream, GuiGame& object){
  if(object.changed){
    stream << object.field << std::endl;
    object.changed = false;
  }
  return stream;
}
