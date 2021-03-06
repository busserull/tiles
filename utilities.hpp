#include <string>

namespace ut{
  int randInclusive(int lower, int upper);

  void shuffleArray(int *arr, int length);

  bool validPort(std::string inputPort);

  void stripNonPortLetters(std::string& port);

  bool validIP(std::string ipAddress);

  void stripNonIPLetters(std::string& ipAddress);

  void formatPlayerName(std::string& playerName);

  void storeIP(std::string ipAddress);

  void storePort(std::string port);

  std::string getLastUsedIP();

  std::string getLastUsedPort();
}
