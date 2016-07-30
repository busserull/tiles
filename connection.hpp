#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include <string>
#include <SFML/Network.hpp>

enum class ConnectionState{
  Host, Client
};

class Connection{
public:
  Connection(ConnectionState state);
  void setPort(std::string port);
  void setIP(std::string ip);
  void connect();
  void send(const sf::Packet& packet);
  void receive(sf::Packet& packet);
private:
  std::string ipAddress;
  std::string port;
  sf::TcpListener listener;
  sf::TcpSocket socket;
  ConnectionState state;
};

#endif
