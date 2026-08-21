#pragma once

#include <iostream>            
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   
#include <cstdlib>

#include "player.h"

class ServerSocket{
	private:
		int server_fd = -1;
		sockaddr_in address{};
		int addrlen = sizeof(address);
		char buffer[BUFFER_SIZE];
	public:
		ServerSocket();
		~ServerSocket();
			
		int get_server_fd();

		void start_server(Player &fd1, Player &fd2);
};



