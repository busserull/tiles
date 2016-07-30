#include "connection.hpp"
#include <stdexcept>

#include <iostream> ///

Connection::Connection(ConnectionState state) : state(state), ipAddress(""), port(""){

}

void Connection::setPort(std::string port){
  this->port = port;
}

void Connection::setIP(std::string ip){
  ipAddress = ip;
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
    std::cout << "Made connection!" << std::endl; ///
  }
}

void Connection::send(const sf::Packet& packet){
  if(socket.send(packet) != sf::Socket::Done){
    throw std::runtime_error("Failed to send packet");
  }
  std::cout << "Packet sent!" << std::endl; ///
}

void Connection::receive(sf::Packet& packet){
  if(socket.receive(packet) != sf::Socket::Done){
    throw std::runtime_error("Failed to receive packet");
  }
  std::cout << "Packet received!" << std::endl; ///
}
