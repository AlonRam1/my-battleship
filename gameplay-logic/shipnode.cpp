#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   
#include "shipnode.h"


ShipNode::ShipNode(int pos[2])
{
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
}

int (&ShipNode::get_pos())[2] { return pos;}
bool ShipNode::get_is_active(){return is_active;}

void ShipNode::set_is_active(bool is_active){this->is_active = is_active;}


