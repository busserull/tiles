#include <iostream>
#include "field.hpp"
#include "guigame.hpp"

namespace{
  const int height = 6; //Y mines
  const int width = 17; //X mines
  const int mines = 10; //Mines placed in total
}

int main(){
  GuiGame game(height, width, mines);
  while(game.isOpen()){
    game.getEvent();
    game.display();
  }
  return 0;
}
