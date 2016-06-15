#include <iostream>
#include "field.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

namespace{
  const int height = 10; //Y mines
  const int width = 25; //X mines
  const int tileSize = 40; //Pixels
  const int borderSize = 2; //Pixels in border
  const sf::Color openColor(120, 120, 120);
  const sf::Color closedColor(50, 50, 50);
}

int main(){
  sf::RenderWindow window(sf::VideoMode(width*tileSize + borderSize, height*tileSize + borderSize),
  "This is a nice window", sf::Style::Titlebar);

  Field *field = new Field(height, width, 5);

  while(window.isOpen()){
    window.clear();
    for(int y = height - 1; y >= 0; y--){
      for(int x = 0; x < width; x++){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        field->toggleOpen(x, y);
    /// *** ///
        for(int i = 0; i < height; i++){
          for(int j = 0; j < width; j++){
            const int tileX = tileSize * j;
            const int tileY = tileSize * (height - i - 1); //So memory (x,y) == graphical (x,y)
            sf::RectangleShape tile;
            tile.setSize(sf::Vector2f(tileSize - borderSize, tileSize - borderSize));
            tile.setFillColor(field->isOpen(j, i) ? openColor : closedColor);
            tile.setPosition(tileX + borderSize, tileY + borderSize);
            window.draw(tile);
          }
        }
    /// *** ///
    window.display();
  }}
  //
  }
  delete[] field;
  field = nullptr;
  return 0;
}
