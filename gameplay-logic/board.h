#pragma once

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
#include "config.h"

class Board{
	private:
		char board[8][8];
		std::vector<Ship> ships;
		int num_of_active_ships = NUM_OF_SHIPS;

	public:
		Board();

		Board(Ship ships[NUM_OF_SHIPS]);

		char (&get_board())[8][8];

		std::vector<Ship>& get_ships();

		int get_num_of_active_ships();

		void set_ships(std::vector<Ship> ships);

		void set_num_of_active_ships(int num_of_active_ships);

		std::string board_to_string(const std::string& message);

		void update_board();

		bool is_space_taken(int h, int v);

		bool hit_or_miss(std::string s);

		void sink_node(std::string s);
		
		bool is_ship_overlap(std::string s, bool is_vertical);
};

