#include <iostream>
#include <chrono>
#include <thread>
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
	// Refresh rate ~60Hz
	std::this_thread::sleep_for(std::chrono::milliseconds(17));
  }
  return 0;
}
