##BATTLESHIP GAME

This is a small implementation of the Battleship board game using POSIX sockets.

##HOW TO PLAY

This is a turn based, two-player game. It launches via the terminal and provides the players with instructions on the actions they can take.
At the start of the game, both players place 3 ships on the board, two of them are two spaces long, and the third is three spaces long.
After both players place their ships on the board, they begin to attack spaces in turns. Each player inputs a coordinate to attack, and is given a message on whether they
hit a target or missed. The game ends after one player hits all the enemy's spaces containing ships.

##REQUIREMENTS

- Linux or another POSIX-compatible operating system
- C++17-compatible compiler
- CMake 3.20 or newer
- Kitty terminal (required by `scripts/run-game.sh`)

##HOW TO BUILD

To build the game, you can run:

cmake -S . -B build
cmake --build build

##HOW TO RUN

The scripts directory hosts 'server.sh' and 'client.sh' to start the game server and clients respectively.
Alternatively, you can run 'test.sh' from the same directory to quickly launch a server and two clients on seperate kitty tabs (requires kitty terminal). 
