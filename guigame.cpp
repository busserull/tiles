#include "guigame.hpp"
#include "utilities.hpp"
#include <stdexcept>

namespace{
  const int tile_size = 40;
  const int border_size = 2;
  const int side_bar_width = tile_size * 4;

  const std::string primary_font = "sansation.ttf";
  const std::string secondary_font = "bebasregular.otf";
  const int max_name_length = 15;

  const sf::Color side_bar_color(182, 228, 109);
  const sf::Color side_bar_mutable_color(99, 192, 242);
  const sf::Color open_tile_color(120, 120, 120);
  const sf::Color closed_tile_color(50, 50, 50);
  const sf::Color mine_color(255, 81, 89);
  const sf::Color flag_color(255, 205, 70);
  const sf::Color opponent_flag_color(99, 192, 242);
  const sf::Color number_colors[9] = {
    sf::Color(0, 0, 0),       // 0
    sf::Color(89, 124, 255),  // 1
    sf::Color(182, 228, 109), // 2
    sf::Color(255, 81, 89),   // 3
    sf::Color(255, 205, 70),  // 4
    sf::Color(89, 124, 255),  // 5
    sf::Color(182, 228, 109), // 6
    sf::Color(255, 81, 89),   // 7
    sf::Color(255, 205, 70)   // 8
  };
}

GuiGame::GuiGame(int height, int width, int mines) : height(height), width(width), mines(mines),
flagsPlaced(0), state(Gamestate::Pending), mode(Playermode::Singleplayer), lastSelectedMultiplayerMode(Playermode::Client), playerName("Babbage"){
  field = Field(height, width, mines);
  if(!PriFont.loadFromFile(primary_font)){
    throw std::runtime_error("Could not load " + primary_font);
  }
  if(!SecFont.loadFromFile(secondary_font)){
    throw std::runtime_error("Could not load " + secondary_font);
  }
  window = new sf::RenderWindow(sf::VideoMode(width * tile_size + border_size + side_bar_width, height * tile_size + border_size),
  "Tiles", sf::Style::Titlebar);
}

GuiGame::~GuiGame(){
  delete window;
  window = nullptr;
}

bool GuiGame::isOpen() const{
  return window->isOpen();
}

void GuiGame::getEvent(){
  sf::Event event;
  while(window->pollEvent(event)){
    switch(event.type){
      case sf::Event::EventType::MouseButtonPressed:
        clickAt(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
        //changed = true; // For operator <<
        break;
      case sf::Event::EventType::KeyPressed:
        switch(event.key.code){
          case sf::Keyboard::Key::Q:
            window->close();
        }
    }
  }
}

void GuiGame::updateGameState(){
  if(field.hasMinesBeenPlaced() == false){
    state = Gamestate::Pending;
  }
  else{
    if(field.hasMinesBeenOpened()){
      state = Gamestate::Lost;
    }
    else if(field.onlyMinesLeft()){
      state = Gamestate::Won;
    }
    else{
      state = Gamestate::Playing;
    }
  }
}

Gamestate GuiGame::getState() const {
  return state;
}

void GuiGame::display(){
  window->clear();
  // Start drawing tiles
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      int tileX = tile_size * x + border_size;
      int tileY = tile_size * (height - y - 1) + border_size;
      sf::RectangleShape tile;
      tile.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
      tile.setFillColor(field.isOpen(x, y) ? open_tile_color : closed_tile_color);
      tile.setPosition(tileX, tileY);
      window->draw(tile);
    }
  }
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      drawLabel(x, y);
    }
  }
  // Stop drawing tiles

  // Start drawing sidebar
  {
    sf::RectangleShape sidepane;
    sidepane.setSize(sf::Vector2f(side_bar_width - border_size, height * tile_size - border_size));
    sidepane.setFillColor(closed_tile_color);
    sidepane.setPosition(width * tile_size + border_size, border_size);
    window->draw(sidepane);

    sf::RectangleShape separator;
    separator.setSize(sf::Vector2f(side_bar_width - 10 * border_size, border_size / 2));
    separator.setFillColor(side_bar_color);
    separator.setPosition(width * tile_size + 5 * border_size, border_size);
    int distance = height * tile_size / 3;
    separator.move(0, distance);
    window->draw(separator);
    separator.move(0, distance);
    window->draw(separator);
  }
  // Start drawing indicator
  {
    std::string indicatorStr;
    switch(state){
      case Gamestate::Won:
        indicatorStr = ": ";
        break;
      case Gamestate::Lost:
        indicatorStr = ":(";
        break;
      case Gamestate::Playing:
        indicatorStr = ":)";
        break;
      case Gamestate::Pending:
        indicatorStr = ":|";
        break;
    }
    sf::Text stateLabel;
    stateLabel.setFont(PriFont);
    stateLabel.setCharacterSize(height * tile_size / 4);
    stateLabel.setStyle(sf::Text::Bold);
    stateLabel.setString(indicatorStr);
    stateLabel.setColor(side_bar_color);
    sf::FloatRect boundingBox = stateLabel.getLocalBounds();
    stateLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    stateLabel.setPosition(width * tile_size + side_bar_width / 2, height * tile_size / 6);
    stateLabel.setRotation(90);
    window->draw(stateLabel);
    if(state == Gamestate::Won){
      sf::Vector2f position = stateLabel.getPosition();
      stateLabel.setString("  D");
      stateLabel.setStyle(sf::Text::Regular);
      stateLabel.setCharacterSize(height * tile_size / 6);
      boundingBox = stateLabel.getLocalBounds();
      stateLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
      stateLabel.setPosition(position);
      stateLabel.scale(0.8, 1.5);
      window->draw(stateLabel);
    }
  }
  // Start drawing time
  {
    std::string secString, minString;
    if(state != Gamestate::Pending){
      int seconds = field.getSecondsSinceStart();
      int minutes = seconds / 60;
      seconds %= 60;
      if(minutes < 10){
        minString.push_back('0');
      }
      minString += std::to_string(minutes);
      if(seconds < 10){
        secString.push_back('0');
      }
      secString += std::to_string(seconds);
      if(minutes > 99){
        minString = "99";
        secString = "++";
      }
    }
    else{
      secString = "00";
      minString = "00";
    }
    sf::Text timeLabel;
    timeLabel.setFont(SecFont);
    timeLabel.setCharacterSize(height * tile_size / 16);
    timeLabel.setStyle(sf::Text::Bold);
    timeLabel.setString(minString + ":" + secString); ///
    timeLabel.setColor(side_bar_color); ///
    sf::FloatRect boundingBox = timeLabel.getLocalBounds();
    timeLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    timeLabel.setPosition(width * tile_size + side_bar_width / 2, height * tile_size / 2);
    window->draw(timeLabel);
  }
  // Start drawing minecounter
  {
    sf::Text mineLabel;
    mineLabel.setFont(SecFont);
    mineLabel.setCharacterSize(height * tile_size / 4);
    mineLabel.setStyle(sf::Text::Bold);
    if(state != Gamestate::Playing){
      mineLabel.setString(std::to_string(mines));
    }
    else{
      mineLabel.setString(std::to_string(mines - flagsPlaced));
    }
    mineLabel.setColor(side_bar_color);
    sf::FloatRect boundingBox = mineLabel.getLocalBounds();
    mineLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    mineLabel.setPosition(width * tile_size + side_bar_width / 2, height * tile_size * 5 / 6);
    window->draw(mineLabel);

    // Blue 'mines' if mine count is mutable, green otherwise
    mineLabel.setString("30"); // Stop 'mines' label from jumping up and down as mines are changed
    boundingBox = mineLabel.getLocalBounds();
    int labelOffset = height * tile_size * 5 / 6 + boundingBox.height * 2 / 3;
    sf::Text minesMutable;
    minesMutable.setFont(SecFont);
    minesMutable.setCharacterSize(height * tile_size / 24);
    if(state != Gamestate::Playing){
      minesMutable.setColor(side_bar_mutable_color);
    }
    else{
      minesMutable.setColor(side_bar_color);
    }
    if(mines - flagsPlaced == 1){
      minesMutable.setString("mine");
    }
    else{
      minesMutable.setString("mines");
    }
    boundingBox = minesMutable.getLocalBounds();
    minesMutable.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top);
    minesMutable.setPosition(width * tile_size + side_bar_width / 2, labelOffset);
    window->draw(minesMutable);
  }
  // Stop drawing sidebar
  window->display();
}

void GuiGame::updateTitle(){
  std::string title;
  switch(mode){
    case Playermode::Singleplayer:
      title = "Singleplayer";
      break;
    case Playermode::Client:
      title = "Client";
      break;
    case Playermode::Host:
      title = "Host";
      break;
  }
  window->setTitle(title);
}

void GuiGame::displayWelcomeScreen(){
  std::string inputName = "Babbage";
  bool inputNameSet = false;
  bool nameChanged = false;
  while(!inputNameSet){
    window->clear();
    drawEmptyBackground();
    drawGameMode();
    drawInputBox("name:", inputName);
    window->display();
    inputNameSet = getUserNameAndMode(inputName, nameChanged);
  }
  playerName = inputName;
  if(mode == Playermode::Host){
    std::string inputPort = "55001";
    bool inputSet = false;
    while(!inputSet){
      window->clear();
      drawEmptyBackground();
      drawInputBox("port:", inputPort);
      inputSet = getUserInput(inputPort, 10);
      ut::stripNonPortLetters(inputPort);
      if(!ut::validPort(inputPort)){
        inputSet = false;
      }
      window->display();
    }
    // Now create Host
  }
  else if(mode == Playermode::Client){
    std::string inputIPAddress = "127.0.0.1";
    std::string inputPort = "55001";
    bool inputSet = false;
    while(!inputSet){
      window->clear();
      drawEmptyBackground();
      drawInputBox("ip:", inputIPAddress);
      inputSet = getUserInput(inputIPAddress, 15);
      ut::stripNonIPLetters(inputIPAddress);
      if(!ut::validIP(inputIPAddress)){
        inputSet = false;
      }
      window->display();
    }
    inputSet = false;
    while(!inputSet){
      window->clear();
      drawEmptyBackground();
      drawInputBox("port:", inputPort);
      inputSet = getUserInput(inputPort, 10);
      ut::stripNonPortLetters(inputPort);
      if(!ut::validPort(inputPort)){
        inputSet = false;
      }
      window->display();
    }
    // Now create Client
  }
}

void GuiGame::clickAt(int x, int y, sf::Mouse::Button button){
  if(x < width * tile_size + border_size){ // Clicked on tile field
    if(state != Gamestate::Won && state != Gamestate::Lost){
      x -= border_size;
      y -= border_size;
      y /= tile_size;
      y = height - y - 1;
      x /= tile_size;
      if(button == sf::Mouse::Button::Left){ // Open
        if(field.isFlagged(x, y) == false || field.getFlagger(x, y) != playerName){
          field.setOpen(x, y);
        }
      }
      else if(button == sf::Mouse::Button::Right){ // Flag
        if(field.isFlagged(x, y)){
          if(field.getFlagger(x, y) == playerName){
            field.toggleFlag(x, y, playerName);
          }
        }
        else{
          if(flagsPlaced < mines && !field.isOpen(x, y) && state != Gamestate::Pending){
            field.toggleFlag(x, y, playerName);
          }
        }
      }
    }
  }
  else{ // Clicked on sidebar
    if(y <= height * tile_size / 3){ // Clicked on indicator field
      state = Gamestate::Pending;
      flagsPlaced = 0;
      field = Field(height, width, mines);
    }
    else if(y > height * tile_size * 2 / 3){ // Clicked on minecounter
      if(state != Gamestate::Playing){ // Allow changing mines
        if(button == sf::Mouse::Button::Left){
          if(mines < width * height - 9)
            mines++;
        }
        else{
          if(mines > 0)
            mines--;
        }
      }
      if(state == Gamestate::Pending){
        field = Field(height, width, mines);
      }
    }
  }
  updateFlagCount();
}

void GuiGame::drawLabel(int x, int y){
  sf::Text label;
  label.setFont(PriFont);
  label.setCharacterSize(tile_size / 2);
  label.setStyle(sf::Text::Bold);
  bool needToDraw = true;
  if(field.isOpen(x, y)){              // tile open
    if(field.isMine(x, y)){
      label.setString("x");
      label.setColor(mine_color);
    }
    else{
      int mines = field.getSurroundingMines(x, y);
      label.setString(std::to_string(mines));
      label.setColor(number_colors[mines]);
      if(mines == 0){
        needToDraw = false;
      }
    }
  }
  else{                               // tile closed
    if(field.isFlagged(x, y)){
      label.setString("F");
      label.setColor((field.getFlagger(x, y) == playerName?flag_color:opponent_flag_color));
    }
    else{
      needToDraw = false;
    }
  }
  sf::FloatRect labBox = label.getLocalBounds();
  label.setOrigin(labBox.left + labBox.width / 2, labBox.top + labBox.height / 2);
  label.setPosition(x * tile_size + tile_size / 2 + border_size, tile_size * (height - y - 1) + tile_size / 2);
  if(needToDraw){
    window->draw(label);
  }
}

void GuiGame::drawEmptyBackground(){
  sf::RectangleShape screen;
  screen.setSize(sf::Vector2f(width * tile_size + side_bar_width - border_size, height * tile_size - border_size));
  screen.setFillColor(closed_tile_color);
  screen.setPosition(border_size, border_size);
  window->draw(screen);
}

void GuiGame::drawGameMode(){
  int midPointUnderNameBox;
  // Draw two separator lines
  {
    int halfNameBoxHeight = height * tile_size / 12;
    int halfHeightOfWindow = height * tile_size / 2;
    midPointUnderNameBox = 3 * halfHeightOfWindow / 2 + halfNameBoxHeight / 2;
    sf::RectangleShape separator;
    separator.setFillColor(open_tile_color);
    separator.setSize(sf::Vector2f(border_size / 2, halfHeightOfWindow - halfNameBoxHeight - 6 * border_size));
    separator.setOrigin(sf::Vector2f(border_size / 4, (halfHeightOfWindow - halfNameBoxHeight - 6 * border_size) / 2));
    separator.setPosition((width * tile_size + side_bar_width) / 2, (halfHeightOfWindow - halfNameBoxHeight) / 2);
    window->draw(separator);
    separator.setPosition((width * tile_size + side_bar_width) / 2, midPointUnderNameBox);
    window->draw(separator);
  }
  // Draw selected game mode
  {
    int midXAxisSquare = (width * tile_size + side_bar_width) / 4;
    sf::Text singleLabel("Singleplayer", SecFont, height * tile_size / 8);
    sf::Text multiLabel("Multiplayer", SecFont, height * tile_size / 8);
    sf::Text hostClientLabel("", SecFont, height * tile_size / 14);
    { // Set correct host or client string
      std::string hostClientString;
      switch(mode){
        case Playermode::Client:
          hostClientString = "Client";
          break;
        case Playermode::Host:
          hostClientString = "Host";
          break;
        default:
          switch(lastSelectedMultiplayerMode){
            case Playermode::Client:
              hostClientString = "Client";
              break;
            case Playermode::Host:
              hostClientString = "Host";
              break;
          }
          break;
      }
      hostClientLabel.setString(hostClientString);
    }
    if(mode == Playermode::Singleplayer){
      singleLabel.setColor(side_bar_color);
      multiLabel.setColor(open_tile_color);
      hostClientLabel.setColor(open_tile_color);
    }
    else{
      singleLabel.setColor(open_tile_color);
      multiLabel.setColor(side_bar_color);
      hostClientLabel.setColor(side_bar_color);
    }
    // Determine origin
    {
      sf::FloatRect boundingBox = singleLabel.getLocalBounds();
      singleLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
      multiLabel.setOrigin(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);

      boundingBox = hostClientLabel.getLocalBounds();
      hostClientLabel.setOrigin(boundingBox.left + boundingBox.width, boundingBox.top);
    }
    singleLabel.setPosition(sf::Vector2f(midXAxisSquare, midPointUnderNameBox));
    multiLabel.setPosition(sf::Vector2f(3 * midXAxisSquare, midPointUnderNameBox));

    sf::FloatRect position = multiLabel.getGlobalBounds();
    hostClientLabel.setPosition(sf::Vector2f(position.left + position.width, position.top + position.height + border_size));

    window->draw(singleLabel);
    window->draw(multiLabel);
    window->draw(hostClientLabel);
  }
}

void GuiGame::drawInputBox(std::string inputType, std::string textField){
  sf::RectangleShape box;
  box.setSize(sf::Vector2f(width * tile_size, height * tile_size / 6));
  box.setFillColor(open_tile_color);
  //box.setOutlineColor(sf::Color::Black);
  //box.setOutlineThickness(border_size);
  box.setOrigin(width * tile_size / 2, height * tile_size / 12);
  box.setPosition((width * tile_size + side_bar_width) / 2, height * tile_size / 2);
  window->draw(box);

  sf::Vector2f position = box.getPosition();
  position -= sf::Vector2f(width * tile_size / 2, height * tile_size / 12);
  sf::Text nameLabel;
  nameLabel.setFont(SecFont);
  nameLabel.setCharacterSize(height * tile_size / 14);
  nameLabel.setStyle(sf::Text::Bold);
  nameLabel.setString(inputType);
  nameLabel.setColor(open_tile_color);
  {
    sf::FloatRect boundingBox = nameLabel.getLocalBounds();
    nameLabel.setOrigin(boundingBox.left, boundingBox.top + boundingBox.height);
  }
  nameLabel.setPosition(position - sf::Vector2f(0, border_size));
  window->draw(nameLabel);

  sf::Text userName;
  userName.setFont(PriFont);
  userName.setCharacterSize(height * tile_size / 10);
  userName.setStyle(sf::Text::Bold);
  userName.setString("M"); // So the name does not jump up and down if it gets changed
  userName.setColor(closed_tile_color);
  {
    sf::FloatRect boundingBox = userName.getLocalBounds();
    userName.setOrigin(boundingBox.left, boundingBox.top + boundingBox.height / 2);
  }
  userName.setString(textField);
  userName.setPosition(position + sf::Vector2f(2 * border_size, height * tile_size / 12));
  window->draw(userName);
}

bool GuiGame::getUserNameAndMode(std::string& inputName, bool& changed){
  sf::Event event;
  while(window->pollEvent(event)){
    if(event.type == sf::Event::EventType::KeyPressed){
      switch(event.key.code){
        case sf::Keyboard::Key::BackSpace:
          if(!changed){
            inputName = "";
            changed = true;
          }
          else if(inputName != ""){
            inputName.pop_back();
          }
          break;
        case sf::Keyboard::Key::Return:
          if(inputName != ""){
            return true;
          }
          break;
        case sf::Keyboard::Key::Right:
          if(mode == Playermode::Singleplayer){
            mode = lastSelectedMultiplayerMode;
          }
          else{ // Had already selected host or client
            if(mode == Playermode::Client){
              lastSelectedMultiplayerMode = Playermode::Client;
              mode = Playermode::Host;
            }
            else{
              lastSelectedMultiplayerMode = Playermode::Host;
              mode = Playermode::Client;
            }
          }
          break;
        case sf::Keyboard::Key::Left:
          if(mode == Playermode::Client){
            lastSelectedMultiplayerMode = Playermode::Client;
          }
          else{
            lastSelectedMultiplayerMode = Playermode::Host;
          }
          mode = Playermode::Singleplayer;
          break;
        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::Down:
          switch(mode){
            case Playermode::Client:
              mode = Playermode::Host;
              lastSelectedMultiplayerMode = Playermode::Client;
              break;
            case Playermode::Host:
              mode = Playermode::Client;
              lastSelectedMultiplayerMode = Playermode::Host;
              break;
          }
          break;
      }
    }
    else if(event.type == sf::Event::EventType::MouseButtonPressed){
      int alongXAxis = event.mouseButton.x;
      if(alongXAxis <= (width * tile_size + side_bar_width) / 2){
        switch(mode){
          case Playermode::Client:
            lastSelectedMultiplayerMode = Playermode::Client;
            break;
          case Playermode::Host:
            lastSelectedMultiplayerMode = Playermode::Host;
            break;
        }
        mode = Playermode::Singleplayer;
      }
      else{
        if(mode == Playermode::Singleplayer){
          mode = lastSelectedMultiplayerMode;
        }
        else{ // Was already either host or client
          if(mode == Playermode::Client){
            lastSelectedMultiplayerMode = Playermode::Client;
            mode = Playermode::Host;
          }
          else{
            lastSelectedMultiplayerMode = Playermode::Host;
            mode = Playermode::Client;
          }
        }
      }
    }
    else if(event.type == sf::Event::EventType::TextEntered){
      if(!changed){
        inputName = "";
        changed = true;
      }
      sf::Event::TextEvent text = event.text;
      if(text.unicode >= 'a' && text.unicode <= 'z'){
        inputName.push_back(text.unicode);
      }
      else if(text.unicode >= 'A' && text.unicode <= 'Z'){
        inputName.push_back(tolower(text.unicode));
      }
    }
  }
  while(inputName.length() > max_name_length){
    inputName.pop_back();
  }
  if(inputName.length() == 1){
    char lower = inputName[0];
    inputName.pop_back();
    inputName.push_back(toupper(lower));
  }
  return false;
}

bool GuiGame::getUserInput(std::string& inputString, int sizeLimit){
  sf::Event event;
  while(window->pollEvent(event)){
    if(event.type == sf::Event::EventType::KeyPressed){
      switch(event.key.code){
        case sf::Keyboard::Key::BackSpace:
          if(inputString != ""){
            inputString.pop_back();
          }
          break;
        case sf::Keyboard::Key::Return:
          return true;
          break;
      }
    }
    else if(event.type == sf::Event::EventType::TextEntered){
      sf::Event::TextEvent text = event.text;
      // Only accept printable ascii characters
      if(text.unicode >= ' ' && text.unicode <= '~' && inputString.length() < sizeLimit){
        inputString.push_back(text.unicode);
      }
    }
  }
  return false;
}

void GuiGame::updateFlagCount(){
  int flags = 0;
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      if(field.isFlagged(x, y) && field.getFlagger(x, y) == playerName){
        flags++;
      }
    }
  }
  flagsPlaced = flags;
}

/*
std::ostream& operator << (std::ostream& stream, GuiGame& object){
  if(object.changed){
    stream << object.field << std::endl;
    object.changed = false;
  }
  return stream;
}
*/
