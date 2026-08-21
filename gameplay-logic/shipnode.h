#pragma once

#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   


class ShipNode{
	private:
		int pos[2];
		bool is_active = true;
	public:
		ShipNode(int pos[2]);
			
		int (&get_pos())[2];
		bool get_is_active();
		
		void set_is_active(bool is_active);
};


