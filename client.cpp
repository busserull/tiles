#include "client.hpp"

Client::Client() : ipAddress("127.0.0.1"), port(55001){
}

Client::Client(std::string ipAddress, int port) : ipAddress(ipAddress), port(port){
}

Client& Client::operator = (const Client& rhs){
  if(&rhs == this){
    return *this;
  }
  ipAddress = rhs.ipAddress;
  port = rhs.port;
}

void Client::connectToHost(std::string playerName){
  socket.connect(ipAddress, port);
  sf::Packet packet;
  packet << playerName;
  socket.send(packet);
}

void Client::receiveCompleteBoard(Field& field){
  sf::Packet packet;
  // Check message
  {
    std::string message;
    packet >> message;
    if(message != "completeBoard"){
      throw message;
    }
  }
  // Height, width and mines
  int height, width, mines;
  packet >> height >> width >> mines;
  field = Field(height, width, mines);
  int tileCount = height * width;
  for(int i = 0; i < tileCount; i++){
    int x, y;
    bool isOpen, isMine, isFlagged;
    std::string flagger;
    packet >> x >> y >> isOpen >> isMine >> isFlagged >> flagger;
    field.setTile(x, y, isOpen, isMine, isFlagged, flagger);
  }
}
