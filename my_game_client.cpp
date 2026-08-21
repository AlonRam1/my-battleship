#include <iostream>  
#include <string>         
#include <cstring>        
#include <sys/socket.h>   
#include <arpa/inet.h>    
#include <unistd.h>       
#include <netinet/in.h>   
#include <cstdlib>
#include <poll.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define NUM_OF_FDS 3

using namespace std;


class ServerSocket
{
	private:
		int server_fd = -1;
		sockaddr_in address{};
		int addrlen = sizeof(address);
		char buffer[BUFFER_SIZE];
		struct pollfd fds[NUM_OF_FDS];
		std::string current_screen{};
	public:
	
		   int getServerFd() { return server_fd; }
		   sockaddr_in getAddress() { return address; }
		   int getAddrLen() { return addrlen; }
		   char* getBuffer() { return buffer; }
		   pollfd(&get_fds())[NUM_OF_FDS]{return fds;}
		   std::string get_current_screen(){return current_screen;}

		   
		   void setServerFd(int fd) { server_fd = fd; }
		   void setAddress(const sockaddr_in& addr) { address = addr; }
		   void setAddrLen(int len) { addrlen = len; }
		   void setBuffer(const char* data, size_t size) { size_t copySize = std::min(size, sizeof(buffer) - 1); std::memcpy(buffer, data, copySize); buffer[copySize] = '\0'; }
		   ServerSocket()
		   {
		   	//initialize socket
			if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				perror("socket assignment failed!");
				exit(EXIT_FAILURE);
			}

			//initialize address
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_family = AF_INET;
			address.sin_port = htons(PORT);

			//initilaize poll array
			fds[0].fd = server_fd;
			fds[0].events = POLLIN;
			fds[1].fd = STDIN_FILENO;
			fds[1].events = POLLIN;
		}

		~ServerSocket()
		{
			if(server_fd != -1)
				close(server_fd);
		}

		void clear_console()
		{
			system("clear");
		}

		void update_screen(const std::string& str)
		{
			//clear console, print the given string, and change current_screen variable
			clear_console();
			std::cout << str << "\n" << endl;
			current_screen = str;
		}

		void connect_to_server()
		{
			std::cout << "connecting to server..." <<endl;
			connect(server_fd, (sockaddr*)&address, (socklen_t)addrlen);
			std::cout << "connected to server!"<<endl;
		}

};


int main()
{
	//connect to server, set up socket and keyboard for polling
	ServerSocket server_socket = ServerSocket();
	server_socket.connect_to_server();

	//gameplay loop
	while(poll(server_socket.get_fds(), 2, -1) > 0)
	{
		int bytes_read;
		if(server_socket.get_fds()[0].revents & POLLIN)
		{
			bytes_read = recv(server_socket.getServerFd(), server_socket.getBuffer(), BUFFER_SIZE, 0);
			if (bytes_read == 0)
			{
			        // server disconnected
			        std::cout << "Server disconnected." << std::endl;
			        break;  // exit the poll loop
			}
			std::string data(server_socket.getBuffer(), bytes_read);
			server_socket.update_screen(data);
		}
		if(server_socket.get_fds()[1].revents & POLLIN)
		{
			if (fgets(server_socket.getBuffer(), BUFFER_SIZE, stdin) == NULL)
				break;
			send(server_socket.getServerFd(), server_socket.getBuffer(), strlen(server_socket.getBuffer()), 0);
		}
	}

	//close connection
	close(server_socket.getServerFd());
	return 0;
}
