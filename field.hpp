#ifndef FIELD_HPP
#define FIELD_HPP
#include <string>
#include <ostream>
#include <ctime>

struct Tile{
  bool isOpen;
  bool isMine;
  bool isFlagged;
  std::string flagger;
  int surrounding;
};

class Field{
public:
  Field();
  Field(int height, int width, int mines);
  Field(const Field& other);
  Field& operator = (const Field& rhs);
  ~Field();
  int getHeight() const;
  int getWidth() const;
  void placeMines(int openX, int openY);
  bool isOpen(int x, int y) const;
  bool isMine(int x, int y) const;
  bool isFlagged(int x, int y) const;
  std::string getFlagger(int x, int y) const;
  void toggleOpen(int x, int y);
  void setOpen(int x, int y);
  void toggleFlag(int x, int y, std::string name);
  int getSurroundingMines(int x, int y) const;
  bool onlyMinesLeft() const;
  bool hasMinesBeenPlaced() const;
  bool hasMinesBeenOpened() const;
  int getSecondsSinceStart() const;
  void setTile(int x, int y, bool isOpen, bool isMine, bool isFlagged, std::string flagger, int surrounding);
  friend std::ostream& operator << (std::ostream& stream, const Field& board);
private:
  void initializeMineCount();
  const Tile& at(int x, int y) const;
  void flushSurrounding(int x, int y);
  bool onBoard(int x, int y) const;
  Tile *field;
  int height;
  int width;
  int mines;
  int openTiles;
  bool minesPlaced;
  bool minesOpened;
  time_t startTime;
  time_t endTime;
  bool endTimeSet;
};

#endif
