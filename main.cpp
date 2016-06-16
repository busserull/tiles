#include <iostream>
#include "field.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

namespace{
  const int height = 5; //Y mines
  const int width = 5; //X mines
  const int mines = 10;
  const int tileSize = 40; //Pixels
  const int borderSize = 2; //Pixels in border
  const sf::Color openColor(120, 120, 120);
  const sf::Color closedColor(50, 50, 50);
}

int main(){
  Field board(height, width, mines);

  std::cout << "Before mine placement" << std::endl;
  std::cout << board << std::endl;

  std::cout << "Opening (0, 0)..." << std::endl;
  board.toggleOpen(0, 0);
  board.placeMines(0, 0);
  std::cout << board << std::endl;

  while(true){
    int x, y;
    std::cout << "(x, y) : ";
    std::cin >> x >> y;
    board.toggleOpen(x, y);
    std::cout << board << std::endl;
  }
  return 0;
}
