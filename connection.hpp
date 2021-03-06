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
  void setConnectionName(std::string name);
  void setSocketBlock(bool block);
  bool connect();
  void send(sf::Packet& packet);
  void send(int x, int y, std::string flagger); // Flag
  void send(int x, int y); // Open
  void send(std::string command); // String commands
  bool receive(sf::Packet& packet);
  std::string getOpponentName() const;
private:
  std::string ipAddress;
  std::string port;
  std::string connectionName;
  std::string opponentName;
  sf::TcpListener listener;
  sf::TcpSocket socket;
  bool alreadyConnected;
  ConnectionState state;
};

#endif
