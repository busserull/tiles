#include "utilities.hpp"
#include <cstdlib>
#include <ctime>

namespace ut{
  int randInclusive(int lower, int upper){
    int num = (rand() % (upper - lower + 1));
    return (num + lower);
  }

  void shuffleArray(int *arr, int length){
    srand(time(nullptr));
    int *i = (arr + length);
    while((i - arr) > 1){
      int index = randInclusive(0, (i - arr - 1));
      int temp = *(i - 1);
      *(i - 1) = *(arr + index);
      *(arr + index) = temp;
      i--;
    }
  }

  bool validPort(std::string inputPort){
    if(inputPort == ""){
      return false;
    }
    for(int i = 0; i < inputPort.length(); i++){
      char number = inputPort[i];
      if(number < '0' || number > '9'){
        return false;
      }
    }
    return true;
  }

  void stripNonPortLetters(std::string& port){
    std::string cleanString;
    for(int i = 0; i < port.length(); i++){
      char letter = port[i];
      if(letter >= '0' && letter <= '9'){
        cleanString.push_back(letter);
      }
    }
    port = cleanString;
  }

  bool validIP(std::string ipAddress){
    if(ipAddress == ""){
      return false;
    }
    // Check if it contains exactly four number fields
    {
      int dotCounter = 0;
      for(int i = 0; i < ipAddress.length(); i++){
        char letter = ipAddress[i];
        if(letter == '.'){
          dotCounter++;
        }
      }
      if(dotCounter != 3){
        return false;
      }
    }
    // Add another dot so the last field gets 'pushed'
    ipAddress.push_back('.');
    std::string sectors[4] = {""};
    int currentSector = 0;
    std::string field;
    for(int i = 0; i < ipAddress.length(); i++){
      char letter = ipAddress[i];
      if(letter != '.'){
        field.push_back(letter);
      }
      else{
        sectors[currentSector] = field;
        field = "";
        currentSector++;
      }
    }
    // See if no fields are empty or overrange
    {
      for(int i = 0; i < 4; i++){
        if(sectors[i] == ""){
          return false;
        }
        else if(sectors[i].length() > 3){
          return false;
        }
        else if(std::stoi(sectors[i]) > 255){
          return false;
        }
      }
    }
    return true;
  }

  void stripNonIPLetters(std::string& ipAddress){
    std::string cleanString;
    for(int i = 0; i < ipAddress.length(); i++){
      char letter = ipAddress[i];
      if(letter >= '0' && letter <= '9'){
        cleanString.push_back(letter);
      }
      else if(letter == '.'){
        cleanString.push_back(letter);
      }
    }
    ipAddress = cleanString;
  }
}
