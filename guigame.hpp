#ifndef GUIGAME_HPP
#define GUIGAME_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Network/Packet.hpp>
#include <string>
#include "field.hpp"
#include "connection.hpp"

enum class Gamestate{
  Won, Lost, Playing, Pending
};

enum class Playermode{
  Singleplayer, Host, Client
};

class GuiGame{
public:
  GuiGame(int height, int width, int mines);
  GuiGame(const GuiGame& other);
  GuiGame& operator = (const GuiGame& rhs);
  ~GuiGame();
  bool isOpen() const;
  void getEvent();
  void updateGameState();
  Gamestate getState() const;
  void display();
  void updateTitle();
  void displayWelcomeScreen();
private:
  void clickAt(int x, int y, sf::Mouse::Button button);
  void drawLabel(int x, int y);
  void drawEmptyBackground();
  void drawGameMode();
  void drawInputBox(std::string inputType,std::string textField);
  bool getUserNameAndMode(std::string& inputName, bool& changed);
  bool getUserInput(std::string& inputString, int sizeLimit);
  void updateFlagCount();
  void processPacket(sf::Packet& packet);
  void remakeWindow();
  int height;
  int width;
  int mines;
  int flagsPlaced;
  Field field;
  Gamestate state;
  Playermode mode;
  Playermode lastSelectedMultiplayerMode;
  std::string playerName;
  bool playerTurn;
  Connection connection;
  sf::Font PriFont;
  sf::Font SecFont;
  sf::RenderWindow *window;
};

#endif
