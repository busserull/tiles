#include "connection.hpp"
#include <stdexcept>

Connection::Connection() : state(ConnectionState::Void), ipAddress(""), port(""), connectionName(""), alreadyConnected(false){

}

Connection::Connection(ConnectionState state) : state(state), ipAddress(""), port(""), connectionName(""), alreadyConnected(false){

}

Connection::Connection(const Connection& other){
  ipAddress = other.ipAddress;
  port = other.port;
  connectionName = other.connectionName;
  opponentName = other.opponentName;
  alreadyConnected = false; // Because socket is non-copyable
  state = other.state;
}

Connection& Connection::operator = (const Connection& rhs){
  if(&rhs == this){
    return *this;
  }
  ipAddress = rhs.ipAddress;
  port = rhs.port;
  connectionName = rhs.connectionName;
  opponentName = rhs.opponentName;
  alreadyConnected = false; // Because socket is non-copyable
  state = rhs.state;
  return *this;
}

void Connection::setPort(std::string port){
  this->port = port;
}

void Connection::setIP(std::string ip){
  ipAddress = ip;
}

void Connection::setConnectionName(std::string name){
  connectionName = name;
}

void Connection::setSocketBlock(bool block){
  socket.setBlocking(block);
}

bool Connection::connect(){
  if(connectionName == ""){
    throw std::runtime_error("Connection name not set");
  }
  if(state == ConnectionState::Host){
    if(listener.listen(std::stoi(port)) != sf::Socket::Done){
      throw std::runtime_error("Cannot listen to port " + port);
    }
    if(listener.accept(socket) != sf::Socket::Done){
      throw std::runtime_error("No client connected");
    }
    bool clientHasDifferentName = false;
    while(!clientHasDifferentName){
      // Send host name
      sf::Packet packet;
      packet << connectionName;
      socket.send(packet);
      // Wait for client to tell if names are the same
      packet.clear();
      socket.receive(packet);
      packet >> clientHasDifferentName;
    }
    // Receive client name
    sf::Packet packet;
    socket.receive(packet);
    packet >> opponentName;
    return true;
  }
  else{
    if(!alreadyConnected){
      sf::Socket::Status status = sf::Socket::Disconnected;
      while(status != sf::Socket::Done){
        status = socket.connect(ipAddress, std::stoi(port));
      }
      alreadyConnected = true;
    }
    sf::Packet packet;
    socket.receive(packet);
    std::string hostName;
    packet >> hostName;
    opponentName = hostName;
    if(hostName == connectionName){
      packet.clear();
      packet << false;
      socket.send(packet);
      return false;
    }
    else{
      packet.clear();
      packet << true;
      socket.send(packet);
      // Send clients name to server
      packet.clear();
      packet << connectionName;
      socket.send(packet);
      return true;
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

void Connection::send(std::string command){
  sf::Packet packet;
  packet << command;
  send(packet);
}

bool Connection::receive(sf::Packet& packet){
  sf::Socket::Status status = socket.receive(packet);
  if(status == sf::Socket::Done){
    return true;
  }
  return false;
}

std::string Connection::getOpponentName() const {
  return opponentName;
}
