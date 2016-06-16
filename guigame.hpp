#ifndef GUIGAME_HPP
#define GUIGAME_HPP
#include <SFML/Graphics.hpp>
#include "field.hpp"

enum class Gamestate{
  WON, LOST, WAITING, INVALID
};

class GuiGame{
public:
  GuiGame(int height, int width, int mines);
  ~GuiGame();
  bool isOpen() const;
  void getEvent();
  void display();
  Gamestate getState();
private:
  int height;
  int width;
  int counter; // getEvent simulation
  Field field;
  sf::Font font;
  sf::RenderWindow *window;
};

#endif
