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
  Field(int height, int width, int mines);
  ~Field();
  void placeMines(int openX, int openY);
  bool isOpen(int x, int y) const;
  void toggleOpen(int x, int y);
  friend std::ostream& operator << (std::ostream& stream, const Field& board);
private:
  Tile *field;
  int height;
  int width;
  int mines;
  const Tile& at(int x, int y) const;
};

#endif
