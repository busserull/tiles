#ifndef SERVER_HPP
#define SERVER_HPP
#include <vector>
#include <string>
#include "hostorclient.hpp"
#include "field.hpp"

class Server : public HostOrClient{
public:
  Server();
  Server(std::string hostName, int listenPort);
  Server& operator = (const Server& rhs);
  void waitForConnection();
  void sendCompleteBoard(const Field& field);
private:
  int listenPort;
  sf::TcpListener listener;
  std::vector<std::string> connectedPlayers;
  sf::TcpSocket socket;
};

#endif
