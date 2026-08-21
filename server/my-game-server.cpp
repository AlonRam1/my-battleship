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

#define PORT 8080
#define BUFFER_SIZE 1024
#define NUM_OF_SHIPS 3

using namespace std;

class ShipNode{
	private:
		int pos[2];
		bool is_active = true;
	public:
		ShipNode(int pos[2])
		{
			this->pos[0] = pos[0];
			this->pos[1] = pos[1];
		}
		//getters
		int (&get_pos())[2] { return pos;}
		bool get_is_active(){return is_active;}
		//setters
		void set_is_active(bool is_active){this->is_active = is_active;}
};

class Ship{
	private:
		vector<ShipNode> nodes;
		bool is_vertical;
		int num_of_active_nodes;
		int length;
	public:
		Ship(vector<ShipNode> nodes, bool is_vertical)
		{
			this->nodes = nodes;
			this->is_vertical = is_vertical;
			this->length = nodes.size();
			this->num_of_active_nodes = nodes.size();
		}
		//getters
		vector<ShipNode>& get_nodes(){return nodes;}
		bool get_is_vertical(){return is_vertical;}
		int get_num_of_active_nodes(){return num_of_active_nodes;}
		int get_length(){return length;}
		//setters
		void set_num_of_active_nodes(int num_of_active_nodes){this->num_of_active_nodes = num_of_active_nodes;}
};

class Board{
	private:
		char board[8][8];
		vector<Ship> ships;
		int num_of_active_ships = NUM_OF_SHIPS;

	public:
		Board()
		{
			//set the board as a matrix filled with the '~' character.
			memset(board, '~', sizeof(board));
		}
		Board(Ship ships[NUM_OF_SHIPS])
		{
			//set the board as a matrix filled with the '~' character.
			memset(board, '~', sizeof(board));
			//initialize ships
			this->ships.push_back(ships[0]);
			this->ships.push_back(ships[1]);
			this->ships.push_back(ships[2]);
		}
		//getters
		char (&get_board())[8][8]{return board;}
		vector<Ship>& get_ships(){return ships;}
		int get_num_of_active_ships(){return num_of_active_ships;}
		//setters
		void set_ships(vector<Ship> ships){this->ships = ships;}
		void set_num_of_active_ships(int num_of_active_ships){this->num_of_active_ships = num_of_active_ships;}

		std::string board_to_string(const std::string& message) {
		    std::ostringstream out;

		    out << "    ";
		    for (int j = 0; j < 8; j++) {
		        out << std::setw(4) << (j + 1);
		    }
		    out << "\n";

		    for (int i = 0; i < 8; i++) {
		        char rowLabel = 'A' + i;
		        out << rowLabel << "   ";

		        for (int j = 0; j < 8; j++) {
		            out << std::setw(4) << board[i][j];
		        }

		        out << "\n";
		    }

		    out << "\n\n";
		    out << message << "\n";   // <-- important

		    return out.str();
		}
		void update_board(){
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
		bool is_space_taken(int h, int v)
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
		bool is_ship_overlap(std::string s, bool is_vertical)
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
		bool hit_or_miss(std::string s)
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

		void sink_node(string s)
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
};

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
		Player()
		{
			//initialize socket address
			address.sin_family = AF_INET;
			address.sin_port = htons(PORT);
		}
		//getters
		int get_client_fd(){return client_fd;}
		sockaddr_in get_address(){return address;}
		int get_addrlen(){return addrlen;}
		char (&get_buffer())[BUFFER_SIZE]{return buffer;}
		Board& get_player_board(){return player_board;}
		//setters
		void set_client_fd(int addr){client_fd = addr;}
		void set_player_board(Board player_board){this->player_board = player_board;}

		~Player()
		{
			if(client_fd != -1)
				close(client_fd);
		}

		std::string take_data()
		{
		    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

		    if (bytes_received <= 0) {
		        return "";
		    }

		    return std::string(buffer, bytes_received);
		}

		void clearBuffer() {
		    memset(buffer, 0, BUFFER_SIZE);
		}

		void update_screen(const std::string& str)
		{
			this->player_board.update_board();
			std::string s = player_board.board_to_string(str);
			clearBuffer();
			send(client_fd, s.c_str(), s.size(), 0);
		}

		void add_ship_to_player(const std::string& str)
		{
			//determine if horizontal or vertical
			bool is_vertical;
			if(str[0] > 64 && str[0] < 73)
				is_vertical = false;
			else
				is_vertical = true;
			//translate string to coordinates, and create nodes
			vector<ShipNode> nodes;
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
			vector<Ship> current_vector = player_board.get_ships();
			current_vector.push_back(ship);
			player_board.set_ships(current_vector);
		}

		bool check_valid_coordinates(const std::string& str, int desired_length)
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
		bool is_valid_target(string s)
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

		void take_target(Player& enemy)
		{
			//take coordinates from player
			string s = "enter coordinates to attack (ex. A4, F6, D8)";
			update_screen(s);
			string coordinates = take_data();
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
		void take_ships(bool is_player2)
		{
			//take ship positions and reprint the board each time
			int bytes_received;
			//first ship(size 2)
			string s = "enter the coordinates of your first ship(length 2). should be in format <letter><num><num> or <num><letter><letter>: ";
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
};

class ServerSocket{
	private:
		int server_fd = -1;
		sockaddr_in address{};
		int addrlen = sizeof(address);
		char buffer[BUFFER_SIZE];
	public:
		ServerSocket()
		{
			//initialize socket
			if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				perror("socket assignment failed!");
				exit(EXIT_FAILURE);
			}
			//initalize socket settings
			int opt = 1;
			int optlen = sizeof(opt);
			if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, optlen) < 0)
			{
				perror("socket settings initialization failed!");
				exit(EXIT_FAILURE);
			}
			//initialize address and bind to socket
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_family = AF_INET;
			address.sin_port = htons(PORT);
			if(bind(server_fd, (sockaddr*)&address, (socklen_t)addrlen) < 0)
			{
				perror("bind failed!");
				exit(EXIT_FAILURE);
			}
		}
		~ServerSocket()
		{
			close(server_fd);
		}
		//getters
		int get_server_fd(){return server_fd;}

		void start_server(Player &fd1, Player &fd2)
		{
			//listen for connections
			if(listen(server_fd, 2) < 0)
			{
				perror("listen failed!");
				exit(EXIT_FAILURE);
			}
			//accept connections and print messages
			std::cout <<"connecting..."<< std::endl;
			sockaddr_in fd1_address = fd1.get_address(), fd2_address = fd2.get_address();
			int fd1_addrlen = fd1.get_addrlen(), fd2_addrlen = fd2.get_addrlen();
			fd1.set_client_fd(accept(server_fd, (struct sockaddr*)&fd1_address, (socklen_t*)&fd1_addrlen));
			std::cout << "client 1 accepted!" << std :: endl;
			fd2.set_client_fd(accept(server_fd, (struct sockaddr*)&fd2_address, (socklen_t*)&fd2_addrlen));
			std::cout << "client 2 accepted!" << std :: endl;
		}
	};

void gameplay_loop(Player& p1, Player& p2)
{
    while(p1.get_player_board().get_num_of_active_ships() > 0 &&
          p2.get_player_board().get_num_of_active_ships() > 0)
    {
        p1.take_target(p2);

        if(p2.get_player_board().get_num_of_active_ships() == 0)
            break;

        p2.take_target(p1);
    }

    string s;
    usleep(100000);
    if(p1.get_player_board().get_num_of_active_ships() == 0)
        s = "player 2 won!";
    else
        s = "player 1 won!";

    p1.update_screen(s);
    p2.update_screen(s);
}

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

























