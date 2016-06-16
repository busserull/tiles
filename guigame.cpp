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

GuiGame::GuiGame(int height, int width, int mines) : height(height), width(width){
  field = Field(height, width, mines);
  if(!font.loadFromFile("sansation.ttf")){
    throw std::runtime_error("Could not load sansation.ttf");
  }
  window = new sf::RenderWindow(sf::VideoMode(width * tile_size + border_size, height * tile_size + border_size),
  "This is a nice window", sf::Style::Titlebar);
  counter = -1; // *** !
}

GuiGame::~GuiGame(){
  delete window;
  window = nullptr;
}

bool GuiGame::isOpen() const{
  return window->isOpen();
}

void GuiGame::getEvent(){
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  counter++;
  if(counter == height * width){
    counter = 0;
  }
  int x = counter % width;
  int y = counter / width;
  field.toggleOpen(x, y);
}

void GuiGame::display(){
  window->clear();

  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      const int tileX = tile_size * x;
      const int tileY = tile_size * (height - y - 1);
      sf::RectangleShape tile;
      tile.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
      tile.setFillColor(field.isOpen(x, y) ? open_tile_color : closed_tile_color);
      tile.setPosition(tileX + border_size, tileY + border_size);
      window->draw(tile);
    }
  }

  window->display();
}

Gamestate GuiGame::getState(){
  return Gamestate::INVALID;
}
