#ifndef GUIGAME_HPP
#define GUIGAME_HPP
#include <SFML/Graphics.hpp>
#include <string>
#include "field.hpp"
#include "server.hpp"
#include "client.hpp"

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
  void displayWelcomeScreen();
  //friend std::ostream& operator << (std::ostream& stream, GuiGame& object); ///
private:
  //bool changed; // For operator <<
  void clickAt(int x, int y, sf::Mouse::Button button);
  void drawLabel(int x, int y);
  void drawEmptyBackground();
  void drawGameMode();
  void drawNameBox(std::string inputName);
  bool getUserNameAndMode(std::string& inputName, bool& changed);
  void updateFlagCount();
  int height;
  int width;
  int mines;
  int flagsPlaced;
  Field field;
  Gamestate state;
  Playermode mode;
  std::string playerName;
  Client connection;
  sf::Font PriFont;
  sf::Font SecFont;
  sf::RenderWindow *window;
};

#endif
