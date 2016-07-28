#ifndef GUIGAME_HPP
#define GUIGAME_HPP
#include <SFML/Graphics.hpp>
#include "field.hpp"

//#include <ostream> // For operator <<

enum class Gamestate{
  Won, Lost, Playing, Pending
};

enum class Playermode{
  Singleplayer, Multiplayer
};

class GuiGame{
public:
  GuiGame(int height, int width, int mines);
  ~GuiGame();
  bool isOpen() const;
  void getEvent();
  void updateGameState();
  Gamestate getState() const;
  void display();
  void updateTitle();
  //friend std::ostream& operator << (std::ostream& stream, GuiGame& object); ///
private:
  //bool changed; // For operator <<
  void clickAt(int x, int y, sf::Mouse::Button button);
  void drawLabel(int x, int y);
  void recordStatistics();
  std::string playerName;
  bool gameOutcomeRecorded;
  int height;
  int width;
  int mines;
  int flagsPlaced;
  Field field;
  Gamestate state;
  Playermode mode;
  sf::Font font;
  sf::RenderWindow *window;
};

#endif
