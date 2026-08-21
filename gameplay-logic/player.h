#include <string>         
#include <cstring>       
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>  
#include "board.h"

#define BUFFER_SIZE 1024

class Player{
	private:
		//socket fields
		int client_fd = -1;
		sockaddr_in address{};
		int addrlen = sizeof(address);
		char buffer[BUFFER_SIZE];
		//player fields
		Board player_board = Board();
	public:
		Player();
		
		int get_client_fd();
		sockaddr_in get_address();
		int get_addrlen();
		char (&get_buffer())[BUFFER_SIZE];
		Board& get_player_board();
		
		void set_client_fd(int addr);
		void set_player_board(Board player_board);

		~Player();
		
		std::string take_data();
	
		void clearBuffer();

		void update_screen(const std::string& str);
	
		void add_ship_to_player(const std::string& str);
	
		bool check_valid_coordinates(const std::string& str, int desired_length);
		
		bool is_valid_target(std::string s);
		
		void take_target(Player& enemy);
	
		void take_ships(bool is_player2);
	};


