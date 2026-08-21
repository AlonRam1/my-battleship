#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>
#include "ship.h"
#include "shipnode.h"




Ship::Ship(vector<ShipNode> nodes, bool is_vertical)
{
	this->nodes = nodes;
	this->is_vertical = is_vertical;
	this->length = nodes.size();
	this->num_of_active_nodes = nodes.size();
}

vector<ShipNode>& Ship::get_nodes(){return nodes;}
bool Ship::get_is_vertical(){return is_vertical;}
int Ship::get_num_of_active_nodes(){return num_of_active_nodes;}
int Ship::get_length(){return length;}

void Ship::set_num_of_active_nodes(int num_of_active_nodes){this->num_of_active_nodes = num_of_active_nodes;}


