#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
#include "hostorclient.hpp"

class Client : public HostOrClient{
public:
  Client();
  Client(std::string ipAddress, int port);
  Client& operator = (const Client& rhs);
  void connectToHost(std::string playerName);
  void receiveCompleteBoard(Field& field);
private:
  std::string ipAddress;
  int port;
  sf::TcpSocket socket;
};

#endif
