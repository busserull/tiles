#ifndef FIELD_HPP
#define FIELD_HPP
#include <string>
#include <ostream>

struct Tile{
  bool isOpen;
  bool isMine;
  bool isFlagged;
  std::string flagger;
};

class Field{
public:
  Field();
  Field(int height, int width, int mines);
  Field(const Field& other);
  Field& operator = (const Field& rhs);
  ~Field();
  void placeMines(int openX, int openY);
  bool isOpen(int x, int y) const;
  bool isMine(int x, int y) const;
  bool isFlagged(int x, int y) const;
  std::string getFlagger(int x, int y) const;
  void toggleOpen(int x, int y);
  void toggleFlag(int x, int y, std::string name);
  int getSurroundingMines(int x, int y);
  friend std::ostream& operator << (std::ostream& stream, const Field& board);
private:
  Tile *field;
  int height;
  int width;
  int mines;
  const Tile& at(int x, int y) const;
};

#endif
