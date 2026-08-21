#include <iostream>       
#include <string>         
#include <cstring>       
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "ship.h"
#include "shipnode.h"

#define NUM_OF_SHIPS 3

class Board{
	private:
		char board[8][8];
		vector<Ship> ships;
		int num_of_active_ships = NUM_OF_SHIPS;

	public:
		Board();	
		char (&get_board())[8][8];

		vector<Ship>& get_ships();

		int get_num_of_active_ships();

		void set_ships(vector<Ship> ships);

		void set_num_of_active_ships(int num_of_active_ships){this->num_of_active_ships = num_of_active_ships;}

		std::string board_to_string(const std::string& message);

		void update_board();
		bool is_space_taken(int h, int v);

		bool hit_or_miss(std::string s);

		void sink_node(string s);
};

