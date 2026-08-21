#include <string>
#include <unistd.h>

#include "server-socket.h"
#include "player.h"
#include "gameplay.h"

using namespace std;

int main() {
	//initialize connection with clients.
	ServerSocket server;
	Player player1 = Player(), player2 = Player();
	server.start_server(player1, player2);
	//begin game (take ship positions)
	std::string s = "(waiting for player 1 to pick ship positions)";
	player2.update_screen(s);
	player1.take_ships(false);
	s = "(waiting for player 2 to pick ship positions)";
	player1.update_screen(s);
	player2.take_ships(true);
	//game loop
	gameplay_loop(player1, player2);
	//after the game ends, close the connection
	close(server.get_server_fd());
	return 0;
}
