#include "field.hpp"
#include "utilities.hpp"
#include <stdexcept>

Field::Field(int height, int width, int mines)
: height(height), width(width), mines(mines) {
  field = new Tile[(height * width)];
  for(int i = 0; i < (height * width); i++){
    field[i].isOpen = false;
    field[i].isMine = false;
    field[i].isFlagged = false;
  }
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
}

bool Field::isOpen(int x, int y){
  return field[(y * width + x)].isOpen;
}

void Field::toggleOpen(int x, int y){
  field[(y * width + x)].isOpen = !field[(y * width + x)].isOpen;
}

std::ostream& operator << (std::ostream& stream, const Field& board){
  for(int i = board.height - 1; i >= 0; i--){
    for(int j = 0; j < board.width; j++){
      stream << (board.field[(i * board.width + j)].isMine ? 'X' : 'O');
    }
    stream << std::endl;
  }
  return stream;
}
