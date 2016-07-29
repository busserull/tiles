#include "field.hpp"
#include "utilities.hpp"
#include <stdexcept>

Field::Field() : height(0), width(0), mines(0), openTiles(0), minesPlaced(false), minesOpened(false), endTimeSet(false) {
  field = nullptr;
}

Field::Field(int height, int width, int mines)
: height(height), width(width), mines(mines), openTiles(0), minesPlaced(false), minesOpened(false), endTimeSet(false) {
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
  openTiles = other.openTiles;
  minesPlaced = other.minesPlaced;
  minesOpened = other.minesOpened;
  startTime = other.startTime;
  endTime = other.endTime;
  endTimeSet = other.endTimeSet;
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
  height = rhs.height;
  width = rhs.width;
  mines = rhs.mines;
  openTiles = rhs.openTiles;
  minesPlaced = rhs.minesPlaced;
  minesOpened = rhs.minesOpened;
  startTime = rhs.startTime;
  endTime = rhs.endTime;
  endTimeSet = rhs.endTimeSet;
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

int Field::getHeight() const {
  return height;
}

int Field::getWidth() const {
  return width;
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
  // Does not count openTiles
  // Does not place mines if board is uninitialized
  // Does not check if tile is mine
}

void Field::setOpen(int x, int y){
  if(minesPlaced == false){
    startTime = time(nullptr);
    placeMines(x, y);
    minesPlaced = true;
  }
  int index = y * width + x;
  if(field[index].isOpen == false){
    openTiles++;
  }
  field[index].isOpen = true;
  if(field[index].isMine){
    minesOpened = true;
    endTime = time(nullptr);
    endTimeSet = true;
    for(int i = 0; i < height * width; i++){
      if(field[i].isMine){
        field[i].isOpen = true;
      }
    }
  }
  if(isMine(x,y) == false && getSurroundingMines(x, y) == 0){
    flushSurrounding(x, y);
  }
  if(onlyMinesLeft()){
    endTime = time(nullptr);
    endTimeSet = true;
    for(int i = 0; i < height * width; i++){
      if(field[i].isMine){
        field[i].isFlagged = true;
      }
    }
  }
}

void Field::toggleFlag(int x, int y, std::string name){
  field[(y * width + x)].isFlagged = !field[(y * width + x)].isFlagged;
  field[(y * width + x)].flagger = name;
}

int Field::getSurroundingMines(int x, int y){
  return field[(y * width + x)].surrounding;
}

bool Field::onlyMinesLeft() const {
  return (height * width - openTiles == mines);
}

bool Field::hasMinesBeenPlaced() const {
  return minesPlaced;
}

bool Field::hasMinesBeenOpened() const {
  return minesOpened;
}

int Field::getSecondsSinceStart() const {
  if(!endTimeSet){
    return difftime(time(nullptr), startTime);
  }
  else{
    return difftime(endTime, startTime);
  }
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

const Tile& Field::at(int x, int y) const{
  return field[(y * width + x)];
}

void Field::flushSurrounding(int x, int y){
  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      int nx = x + i;
      int ny = y + j;
      if(onBoard(nx, ny) && isOpen(nx, ny) == false){
        int index = ny * width + nx;
        field[index].isFlagged = false;
        field[index].isOpen = true;
        openTiles++;
        if(getSurroundingMines(nx, ny) == 0){
          flushSurrounding(nx, ny);
        }
      }
    }
  }
}

bool Field::onBoard(int x, int y) const {
  if(x >= 0 && x < width && y >= 0 && y < height){
    return true;
  }
  return false;
}
