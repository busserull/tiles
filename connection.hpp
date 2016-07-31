#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include <string>
#include <SFML/Network.hpp>

enum class ConnectionState{
  Host, Client, Void
};

class Connection{
public:
  Connection();
  Connection(ConnectionState state);
  Connection(const Connection& other);
  Connection& operator = (const Connection& rhs);
  void setPort(std::string port);
  void setIP(std::string ip);
  void setSocketBlock(bool block);
  void connect();
  void send(sf::Packet& packet);
  void send(int x, int y, std::string flagger); // Flag
  void send(int x, int y); // Open
  void send(std::string command); // String commands
  bool receive(sf::Packet& packet);
private:
  std::string ipAddress;
  std::string port;
  sf::TcpListener listener;
  sf::TcpSocket socket;
  ConnectionState state;
};

#endif
