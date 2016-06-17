#include <iostream>
#include "field.hpp"
#include "guigame.hpp"

namespace{
  const int height = 10; //Y mines
  const int width = 15; //X mines
  const int mines = 141; //Mines placed in total
}

int main(){
  GuiGame game(height, width, mines);
  while(game.isOpen()){
    game.getEvent();
    //std::cout << game;
    game.display();
  }
  return 0;
}
