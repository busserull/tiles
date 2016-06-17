#include "field.hpp"
#include "utilities.hpp"
#include <stdexcept>

Field::Field() : height(0), width(0), mines(0) {
  field = nullptr;
}

Field::Field(int height, int width, int mines)
: height(height), width(width), mines(mines) {
  field = new Tile[(height * width)];
  for(int i = 0; i < (height * width); i++){
    field[i].isOpen = false;
    field[i].isMine = false;
    field[i].isFlagged = false;
  }
}

Field::Field(const Field& other){
  height = other.height;
  width = other.width;
  mines = other.mines;
  field = new Tile[(height * width)];
  for(int i = 0; i < (height * width); i++){
    field[i] = other.field[i];
  }
}

Field& Field::operator = (const Field& rhs){
  if(this == &rhs){
    return (*this);
  }
  delete[] field;
  mines = rhs.mines;
  width = rhs.width;
  height = rhs.height;
  field = new Tile[(height * width)];
  for(int i = 0; i < (height * width); i++){
    field[i] = rhs.field[i];
  }
  return (*this);
}

Field::~Field(){
  delete[] field;
  field = nullptr;
}

void Field::placeMines(int openX, int openY){
  int *fieldArray = new int[(width * height)];
  for(int i = 0; i < (width * height); i++){
    fieldArray[i] = i;
  }

  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      int x = openX + i;
      int y = openY + j;
      if(x >= 0 && x < width && y >= 0 && y < height){
        int mineFreeTile = (x + width * y);
        fieldArray[mineFreeTile] = -1;
      }
    }
  }
  ut::shuffleArray(fieldArray, (width * height));
  int index = 0;
  int minesLeft = mines;
  while(minesLeft){
    if(index > (width * height - 1)){
      throw std::out_of_range("Too many mines for field.");
    }
    if(fieldArray[index] != -1){
      int x = fieldArray[index] % width;
      int y = fieldArray[index] / width;
      field[(y * width + x)].isMine = true;
      minesLeft--;
    }
    index++;
  }
  delete[] fieldArray;
  initializeMineCount();
}

bool Field::isOpen(int x, int y) const{
  return field[(y * width + x)].isOpen;
}

bool Field::isMine(int x, int y) const{
  return field[(y * width + x)].isMine;
}

bool Field::isFlagged(int x, int y) const{
  return field[(y * width + x)].isFlagged;
}

std::string Field::getFlagger(int x, int y) const{
  return field[(y * width + x)].flagger;
}

void Field::toggleOpen(int x, int y){
  field[(y * width + x)].isOpen = !field[(y * width + x)].isOpen;
}

void Field::setOpen(int x, int y){
  field[(y * width + x)].isOpen = true;
}

void Field::toggleFlag(int x, int y, std::string name){
  field[(y * width + x)].isFlagged = !field[(y * width + x)].isFlagged;
  field[(y * width + x)].flagger = name;
}

int Field::getSurroundingMines(int x, int y){
  return field[(y * width + x)].surrounding;
}

std::ostream& operator << (std::ostream& stream, const Field& board){
  for(int y = board.height - 1; y >= 0; y--){
    for(int x = 0; x < board.width; x++){
      if(board.at(x, y).isMine && !board.at(x, y).isOpen){
        stream << 'm';
      }
      else if(board.at(x, y).isMine && board.at(x, y).isOpen){
        stream << 'M';
      }
      else if(board.at(x, y).isOpen){
        stream << '-';
      }
      else{
        stream << 'O';
      }
    }
    stream << std::endl;
  }
  return stream;
}

const Tile& Field::at(int x, int y) const{
  return field[(y * width + x)];
}

void Field::initializeMineCount(){
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      int mineCount = 0;
      for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
          int col = x + i;
          int row = y + j;
          if(col >= 0 && col < width && row >= 0 && row < height && (i != 0 || j != 0)){
            mineCount += at(col, row).isMine;
          }
        }
      }
      field[(y * width + x)].surrounding = mineCount;
    }
  }
}
