#include <iostream>
#include "field.hpp"
#include "guigame.hpp"

namespace{
  const int height = 5; //Y mines
  const int width = 5; //X mines
  const int mines = 3; //Mines placed in total
}

int main(){
  GuiGame game(height, width, mines);
  while(game.isOpen()){
    game.getEvent();
    //std::cout << game;
    game.display();
    game.updateTitle();
  }
  return 0;
}
