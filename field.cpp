#include "field.hpp"
#include "utilities.hpp"
#include <stdexcept>

Field::Field(int height, int width, int mines)
: height(height), width(width), mines(mines) {
  field = new Tile*[width];
  for(int i = 0; i < width; i++){
    field[i] = new Tile[height];
  }
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      field[i][j].isOpen = false;
      field[i][j].isMine = false;
      field[i][j].isFlagged = false;
    }
  }
}

Field::~Field(){
  for(int i = 0; i < width; i++){
    delete[] field[i];
  }
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
      field[x][y].isMine = true;
      minesLeft--;
    }
    index++;
  }
  delete[] fieldArray;
}

bool Field::isOpen(int x, int y){
  return field[x][y].isOpen;
}

void Field::toggleOpen(int x, int y){
  field[x][y].isOpen = !field[x][y].isOpen;
}

std::ostream& operator << (std::ostream& stream, const Field& board){
  for(int i = board.height - 1; i >= 0; i--){
    for(int j = 0; j < board.width; j++){
      stream << (board.field[j][i].isMine ? 'X' : 'O');
    }
    stream << std::endl;
  }
  return stream;
}
