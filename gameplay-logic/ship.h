#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   
#include "shipnode.h"


class Ship{
	private:
		vector<ShipNode> nodes;
		bool is_vertical;
		int num_of_active_nodes;
		int length;
	public:
		Ship(vector<ShipNode> nodes, bool is_vertical);
			
		vector<ShipNode>& get_nodes();
		bool get_is_vertical();
		int get_num_of_active_nodes();
		int get_length();
		
		void set_num_of_active_nodes(int num_of_active_nodes);
};

