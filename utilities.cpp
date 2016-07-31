#include "utilities.hpp"
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>

namespace{
  const std::string ip_store_file = ".lastIP.txt";
  const std::string port_store_file = ".lastPort.txt";
}

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

  void formatPlayerName(std::string& playerName){
    std::string onlyLetters;
    for(int i = 0; i < playerName.length(); i++){
      char letter = playerName[i];
      letter = tolower(letter);
      if(letter >= 'a' && letter <= 'z'){
        onlyLetters.push_back(letter);
      }
    }
    if(onlyLetters.length() > 0){
      onlyLetters[0] = toupper(onlyLetters[0]);
    }
    playerName = onlyLetters;
  }

  void storeIP(std::string ipAddress){
    std::ofstream out(ip_store_file);
    out << ipAddress << std::endl;
    out.close();
  }

  void storePort(std::string port){
    std::ofstream out(port_store_file);
    out << port << std::endl;
    out.close();
  }

  std::string getLastUsedIP(){
    std::ifstream in(ip_store_file);
    if(in.fail()){
      return "127.0.0.1";
    }
    std::string lastIP;
    getline(in, lastIP);
    return lastIP;
    in.close();
  }

  std::string getLastUsedPort(){
    std::ifstream in(port_store_file);
    if(in.fail()){
      return "55001";
    }
    std::string lastPort;
    getline(in, lastPort);
    return lastPort;
    in.close();
  }
}
