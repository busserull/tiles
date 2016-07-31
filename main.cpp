#include <iostream>
#include "field.hpp"
#include "guigame.hpp"

namespace{
  const int height = 10; //Y mines
  const int width = 10; //X mines
  const int mines = 10; //Mines placed in total
}

int main(){
  GuiGame game(height, width, mines);
  game.displayWelcomeScreen();
  while(game.isOpen()){
    game.getEvent();
    game.display();
    game.updateGameState();
  }
  return 0;
}
