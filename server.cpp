#include "server.hpp"

Server::Server(std::string hostName, int listenPort) : listenPort(listenPort){
  connectedPlayers.push_back(hostName);
}

void Server::waitForConnection(){
  listener.listen(listenPort);
  listener.accept(socket);
  // Look for name collision
  sf::Packet packet;
  socket.receive(packet);
  std::string name;
  packet >> name;
  bool validName = true;
  for(int i = 0; i < connectedPlayers.size(); i++){
    if(name == connectedPlayers[i]){
      validName = false;
      listener.close();
    }
  }
  if(validName){
    connectedPlayers.push_back(name);
  }
}

void Server::sendCompleteBoard(const Field& field){
  sf::Packet packet;
  packet << "completeBoard";
  sf::Uint32 height, width;
  height = field.getHeight();
  width = field.getWidth();
  packet << height << width;

  // (x, y, isOpen, isMine, isFlagged, flagger, surrounding)
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      bool isOpen = field.isOpen(x, y);
      bool isMine = field.isMine(x, y);
      bool isFlagged = field.isFlagged(x, y);
      std::string flagger = field.getFlagger(x, y);
      sf::Uint8 surrounding = field.getSurroundingMines(x, y);
      sf::Uint32 xPos = x;
      sf::Uint32 yPos = y;
      packet << xPos << yPos << isOpen << isMine << isFlagged
      << flagger << surrounding;
    }
  }
  socket.send(packet);
}
