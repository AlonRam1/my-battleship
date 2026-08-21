#include <iostream>            
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>       
#include <netinet/in.h>   
#include <cstdlib>

#include "server-socket.h"

ServerSocket::ServerSocket()
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
ServerSocket::~ServerSocket()
{
	close(server_fd);
}

int ServerSocket::get_server_fd(){return server_fd;}

void ServerSocket::start_server(Player &fd1, Player &fd2)
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



