#include "connection.hpp"
#include <stdexcept>

Connection::Connection() : state(ConnectionState::Void), ipAddress(""), port(""){

}

Connection::Connection(ConnectionState state) : state(state), ipAddress(""), port(""){

}

Connection::Connection(const Connection& other){
  ipAddress = other.ipAddress;
  port = other.port;
  state = other.state;
}

Connection& Connection::operator = (const Connection& rhs){
  if(&rhs == this){
    return *this;
  }
  ipAddress = rhs.ipAddress;
  port = rhs.port;
  state = rhs.state;
  return *this;
}

void Connection::setPort(std::string port){
  this->port = port;
}

void Connection::setIP(std::string ip){
  ipAddress = ip;
}

void Connection::setSocketBlock(bool block){
  socket.setBlocking(block);
}

void Connection::connect(){
  if(state == ConnectionState::Host){
    if(listener.listen(std::stoi(port)) != sf::Socket::Done){
      throw std::runtime_error("Cannot listen to port " + port);
    }
    if(listener.accept(socket) != sf::Socket::Done){
      throw std::runtime_error("No client connected");
    }
  }
  else{
    sf::Socket::Status status = sf::Socket::Disconnected;
    while(status != sf::Socket::Done){
      status = socket.connect(ipAddress, std::stoi(port));
    }
  }
}

void Connection::send(sf::Packet& packet){
  if(socket.send(packet) != sf::Socket::Done){
    throw std::runtime_error("Failed to send packet");
  }
}

// Flag
void Connection::send(int x, int y, std::string flagger){
  if(flagger == ""){
    throw std::runtime_error("No flagger name given");
  }
  sf::Uint32 xPos, yPos;
  xPos = x;
  yPos = y;
  sf::Packet packet;
  packet << "flag" << flagger << xPos << yPos;
  send(packet);
}

// Open
void Connection::send(int x, int y){
  sf::Uint32 xPos, yPos;
  xPos = x;
  yPos = y;
  sf::Packet packet;
  packet << "open" << xPos << yPos;
  send(packet);
}

bool Connection::receive(sf::Packet& packet){
  sf::Socket::Status status = socket.receive(packet);
  if(status == sf::Socket::Done){
    return true;
  }
  return false;
}
