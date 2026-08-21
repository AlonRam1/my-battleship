#include <iostream>       
#include <string.h>         
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   
#include <iomanip>
#include "board.h"
#include "ship.h"
#include "shipnode.h"
		
	
Board::Board()
{
	//set the b
	//oard as a matrix filled with the '~' character.
	memset(board, '~', sizeof(board));
}

Board::Board(Ship ships[NUM_OF_SHIPS])
{
	//set the board as a matrix filled with the '~' character.
	memset(board, '~', sizeof(board));
	//initialize ships
	this->ships.push_back(ships[0]);
	this->ships.push_back(ships[1]);
	this->ships.push_back(ships[2]);
}
		
char (&get_board())[8][8]{return board;}
vector<Ship>& get_ships(){return ships;}
int get_num_of_active_ships(){return num_of_active_ships;}
		
void set_ships(vector<Ship> ships){this->ships = ships;}
void set_num_of_active_ships(int num_of_active_ships){this->num_of_active_ships = num_of_active_ships;}

std::string board_to_string(const std::string& message) {
	 std::ostringstream out;

	 out << "    ";
	 for (int j = 0; j < 8; j++) 
	 {
	      out << std::setw(4) << (j + 1);
	 }
	 out << "\n";

	 for (int i = 0; i < 8; i++)
	 {
	    char rowLabel = 'A' + i;
	   out << rowLabel << "   ";

	   for (int j = 0; j < 8; j++)
	   {
	       out << std::setw(4) << board[i][j];
	   }

	   out << "\n";
	 }

	out << "\n\n";
	 out << message << "\n";   // <-- important

	 return out.str();
}
void Board::update_board(){
	//iterate through all the ships on the board
	for(int i  = 0; i < ships.size(); i++)
	{
		//for each ship node, check it's position and place it on the board
		for(int j = 0; j < ships[i].get_length(); j++)
		{
			ShipNode current_node = ships[i].get_nodes().at(j);
			int h = current_node.get_pos()[0], v = current_node.get_pos()[1];
			if(current_node.get_is_active())
			{
				board[h][v] = 'S';
			}
			else
			{
				board[h][v] = 'F';
			}
		}
	}
}
bool Board::is_space_taken(int h, int v)
{
	//iterate through all ship nodes and see if one of them matches the given coordinates
	for(int i = 0; i < ships.size() ; i++)
	{
		for(int j = 0; j < ships[i].get_nodes().size(); j++)
		{
			int pos_h = ships[i].get_nodes()[j].get_pos()[0], pos_v = ships[i].get_nodes()[j].get_pos()[1];
			if(h == pos_h && v == pos_v)
				return true;
		}
	}
	return false;
}
bool Board::is_ship_overlap(std::string s, bool is_vertical)
{
	//translate given string to a list of coordinates, and check each one if taken
	int c1, c2, c3;
	if(is_vertical)
	{
		c1 = s[0] - '1';
		c2 = s[1] - 'A';
		c3 = s[2] - 'A';
		for(int i = c2 ; i <= c3 ; i++)
		{
			if(is_space_taken(i, c1))
				return true;
		}
	}
	else
	{
		c1 = s[0] - 'A';
		c2 = s[1] - '1';
		c3 = s[2] - '1';
		for(int i = c2 ; i <= c3 ; i++)
		{
			if(is_space_taken(c1, i))
				return true;
		}
	}
	return false;
}

bool Board::hit_or_miss(std::string s)
{
	//translate given string to coordinates
	int h = s[0] - 'A', v = s[1] - '1';
	//iterate through all ship nodes on the board, and check if there is an active one in the given coordinates
	for(int i = 0; i < ships.size(); i++)
	{
		vector<ShipNode>& current_nodes = ships[i].get_nodes();
		for(int j = 0; j < current_nodes.size(); j++)
		{
			int h_pos = current_nodes[j].get_pos()[0], v_pos = current_nodes[j].get_pos()[1];
			bool pos_match = (h == h_pos)&&(v == v_pos);
			if(pos_match)
			{
				if(current_nodes[j].get_is_active())
					return true;
				return false;
			}
		}
	}
	return false;
}

void Board::sink_node(std::string s)
{
	//translate given string to coordinates
	int h = s[0] - 'A', v = s[1] - '1';
	//iterate through the ship nodes on the board, and change the activity of the relevant node
	for(int i = 0; i < ships.size(); i++)
	{
		vector<ShipNode>& current_nodes = ships[i].get_nodes();
		for(int j = 0; j < current_nodes.size(); j++)
		{
			int h_pos = current_nodes[j].get_pos()[0], v_pos = current_nodes[j].get_pos()[1];
			bool pos_match = (h == h_pos)&&(v == v_pos);
			if(pos_match)
			{
				current_nodes[j].set_is_active(false);
				//update ship and board's active nodes status
				ships[i].set_num_of_active_nodes(ships[i].get_num_of_active_nodes() - 1);
				if(ships[i].get_num_of_active_nodes() <= 0)
					num_of_active_ships -= 1;
			}
		}
	}
}


