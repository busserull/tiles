## A simple game called 'Tiles'
Tiles is a minesweeper game for one or two people.

### Interface
When first starting the game, it will display a welcome
screen - this is where you choose between single- or
multiplayer and set your name. Both the keyboard arrows
and clicking on either side of the screen can be used to
set the game mode. You are only allowed to have a name that
consists entirely of english letter - where the first letter
is uppercase and the remaining are lowercase; the game
automatically enforces this.

If you chose multiplayer as client, you are not allowed to
have the same name as the host - the game will ask you to
set a new one if this occurs.

When the game starts, you will see a field of tiles underneath
which is either a mine or not. A left click will open the clicked
tile, whereas a right click will flag the clicked tile as long as
the game has started - indicated by a ':)' in the top right
corner. A tile that has been flagged must be unflagged before it
will open, so as to avoid accidental tile openings. The amount
of flags a player can place is equal to the amount of mines
in play in any given game.

A tile will show how many mines that are in the surrounding eight
tiles - the ones immediately adjacent to it, as well as its 
diagonals. A tiles will show no numer if it has no mines surrounding
it - and an 'x' if it is a tile itself.
If a tile with no surrounding mines has been opened, it will open
all the eight tiles surrounding it as well. You are always guaranteed
to open a tile with no surrounding mines upon the start of a new
game.

The amount of mines that are in
play can be seen in the bottom right corner - on the sidebar.
Whenever the 'Mines' in this corner is blue, the amount of mines
in play can be changed. This is done by left clicking the mines
counter to increase, or right clicking the mines counter to
decrease. If the 'Mines' label is green, the mines cannot be
changed. A client to a host can never change the mines in play.

Above the mines counter in the bottom right corner the game
time is displayed. It starts as soon as the first move in
a game is made - and stops if the game is either lost or won.

In the top right corner is the gamestate indicator, it can have
the four following faces:
* :| - The gamestate is pending, no move has been made yet.
* :) - The game is currently being played.
* :( - A mine has been opened and the game is lost.
* :D - All non-mine tiles have been opened and the game is won.
If at any time this indicator field is clicked, the current game
will end and a new game begin. A client to a host can never
begin a new game.

If the game is a multiplayer game, players will take turns
opening tiles. Flags placed by either player is visible to both
players - your own showing up as orange, and the other player's
as blue. You can not open a tile that you have flagged before
unflagging it, but you can ignore the other players flags and
directly open those tiles - though you cannot unflag them.
The titlebar of the window will change to display whose turn
it is at any given time. Even though you can only open tiles
when it is your turn, you are free to flag closed tiles
whenever you wish.

*Additional controls*
Q will at any time except for text entry close the window.
If you are either playing singleplayer or as host, the
board can be resized with the keyboard arrows as follows
as long as no game has been started:
* Up - Decrease height by one tile.
* Down - Increase height by one tile.
* Left - Decrease width by one tile.
* Right - Increase width by one tile.

### Description
The game lets the player choose between Singleplayer,
Multiplayer host, or Multiplayer client - as well as
choosing a name.

Both modes, single- and multiplayer are almost done,
though some features remain unimplemented. There is
also a little bit of polish to be done.

### Coming features
* File statistics
* Sudden-death for multiplayer
* Store last used name and IP/port

### 3rd party components
SFML is required as it is used for both graphics and
network. Apart from that, the only other libraries used
are either part of the C++ standard or written for
this game.

The two fonts used in the game - Bebas Neue, and Sansation
are both freeware, but dontations can be made on
fontfabric.com/bebas-neue and dafont.com/sansation.font
