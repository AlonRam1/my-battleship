#include <string>         
#include <cstring>       
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>  
#include "player.h"
#include "ship.h"
#include "shipnode.h"


Player::Player()
{
	//initialize socket address
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
}

int Player::get_client_fd(){return client_fd;}
sockaddr_in Player::get_address(){return address;}
int Player::get_addrlen(){return addrlen;}
char (&Player::get_buffer())[BUFFER_SIZE]{return buffer;}
Board& Player::get_player_board(){return player_board;}

void Player::set_client_fd(int addr){client_fd = addr;}
void Player::set_player_board(Board player_board){this->player_board = player_board;}

Player::~Player()
{
	if(client_fd != -1)
		close(client_fd);
}

std::string Player::take_data()
{
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
	return "";
    }

    return std::string(buffer, bytes_received);
}

void Player::clearBuffer() {
    memset(buffer, 0, BUFFER_SIZE);
}

void Player::update_screen(const std::string& str)
{
	this->player_board.update_board();
	std::string s = player_board.board_to_string(str);
	clearBuffer();
	send(client_fd, s.c_str(), s.size(), 0);
}

void Player::add_ship_to_player(const std::string& str)
{
	//determine if horizontal or vertical
	bool is_vertical;
	if(str[0] > 64 && str[0] < 73)
		is_vertical = false;
	else
		is_vertical = true;
	//translate string to coordinates, and create nodes
	std::vector<ShipNode> nodes;
	if(is_vertical)
	{
		int h = str[0] - '1';
		int v_start = str[1] - 'A', v_end = str[2] - 'A';
		for(int i = v_start; i <= v_end; i++)
		{
			int pos[] = {i,h};
			nodes.push_back(ShipNode(pos));
		}
	}
	else
	{
		int v = str[0] - 'A';
		int h_start = str[1] - '1', h_end = str[2] - '1';
		for(int i = h_start; i <= h_end; i++)
		{
			int pos[] = {v, i};
			nodes.push_back(ShipNode(pos));
		}
	}
	//initialize ship
	Ship ship = Ship(nodes, is_vertical);
	//add ship to player's board
	std::vector<Ship> current_vector = player_board.get_ships();
	current_vector.push_back(ship);
	player_board.set_ships(current_vector);
}

bool Player::check_valid_coordinates(const std::string& str, int desired_length)
{
	//check valid input length
	if(!(str.length() == 4))
		return false;
	//check if each character in valid letter range, and if ship is of valid length
	//first character is a letter
	if(str[0] > 64 && str[0] < 73)
	{
		bool cond = (str[1] > 48 && str[1] < 57) && (str[2] > 48 && str[2] < 57);
		if(!cond)
			return false;
		int length = str[2] - str[1] + 1;
		if(length != desired_length)
			return false;
		//check if ship overlaps with another ship
		if(player_board.is_ship_overlap(str, false))
			return false;
		return true;
	}
	//first character is a number
	else if(str[0] > 48 && str[0] < 57)
	{
		bool cond = (str[1] > 64 && str[1] < 73) && (str[2] > 64 && str[2] < 73);
		if(!cond)
			return false;
		int length = str[2] - str[1] + 1;
		if(length != desired_length)
			return false;
		//check if ship overlaps with another ship
		if(player_board.is_ship_overlap(str, true))
			return false;
		return true;
	}
	else
		return false;
}
bool Player::is_valid_target(std::string s)
{
	//check if valid length
	if(s.size() != 3)
		return false;
	//check if each character is in valid range
	bool is_h_valid = (s[0] > 64)&&(s[0] < 73);
	bool is_v_valid = (s[1] > 48)&&(s[1] < 57);
	if(!(is_h_valid && is_v_valid))
		return false;
	return true;
}

void Player::take_target(Player& enemy)
{
	//take coordinates from player
	std::string s = "enter coordinates to attack (ex. A4, F6, D8)";
	update_screen(s);
	std::string coordinates = take_data();
	//check if valid coordinates
	bool flag = is_valid_target(coordinates);
	//enter loop until player gives valid coordinates
	while(!flag)
	{
		s = "invalid coordinates!\nenter coordinates to attack (ex. A4, F6, D8)";
		update_screen(s);
		coordinates = take_data();
		flag = is_valid_target(coordinates);
	}
	//check if hit or miss
	bool is_hit = enemy.get_player_board().hit_or_miss(coordinates);
	//notify player and update the ship/board
	if(is_hit)
	{
		enemy.get_player_board().sink_node(coordinates);
		s = "hit!\n(waiting for enemy to pick target)";
		update_screen(s);
	}
	else
	{
		s = "miss!\n(waiting for enemy to pick target)";
		update_screen(s);
	}
}
void Player::take_ships(bool is_player2)
{
	//take ship positions and reprint the board each time
	int bytes_received;
	//first ship(size 2)
	std::string s = "enter the coordinates of your first ship(length 2). should be in format <letter><num><num> or <num><letter><letter>: ";
	update_screen(s);
	std::string coordinates1 = take_data();
	bool flag = check_valid_coordinates(coordinates1, 2);
	while(!flag)
	{
		s = "invalid coordinates!\nenter the coordinates of your first ship(length 2). should be in format <letter><num><num> or <num><letter><letter>: ";
		update_screen(s);
		coordinates1 = take_data();
		flag = check_valid_coordinates(coordinates1, 2);
	}
	add_ship_to_player(coordinates1);
	//second ship(size 2)
	s = "enter the coordinates of your second ship(length 2). should be in format <letter><num><num> or <num><letter><letter>: ";
	update_screen(s);
	std::string coordinates2 = take_data();
	flag = check_valid_coordinates(coordinates2, 2);
	while(!flag)
	{
		s = "invalid coordinates!\nenter the coordinates of your second ship(length 2). should be in format <letter><num><num> or <num><letter><letter>: ";
		update_screen(s);
		coordinates2 = take_data();
		flag = check_valid_coordinates(coordinates2, 2);
	}
	add_ship_to_player(coordinates2);
	//third ship(size 3)
	s = "enter the coordinates of your third ship(length 3). should be in format <letter><num><num> or <num><letter><letter>: ";
	update_screen(s);
	std::string coordinates3 = take_data();
	flag = check_valid_coordinates(coordinates3, 3);
	while(!flag)
	{
		s = "invalid coordinates!\nenter the coordinates of your third ship(length 2). should be in format <letter><num><num> or <num><letter><letter>: ";
		update_screen(s);
		coordinates3 = take_data();
		flag = check_valid_coordinates(coordinates3, 3);
	}
	add_ship_to_player(coordinates3);
	//NOTE: we only send this message to player two because player one is supposed to receive a different message, and instantly changing the screen doesn't work because of refresh issues
	if(is_player2)
		update_screen("(waiting for enemy to pick target)");
}


