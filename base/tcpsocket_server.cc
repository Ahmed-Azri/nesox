#include "tcpsocket.h"
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>

#define LOG(message) log(message)

inline int log(char* message) { return std::cout << std::string(message) << std::endl; }

static const int block_size = 0xffff;

int serverfunction();

int serverfunction(){
	LOG("server: start processing!");

	Nesox::TCPSocket server_listening;
	Nesox::TCPSocket server_connected;

	server_listening.Bind("127.0.0.1", 6655);

	char clientip[0xff];
	short clientport;
	LOG("server: listening");
	server_listening.Listen(5);
	server_listening.Accept(&server_connected, clientip, &clientport);
	LOG("server: connected");

	char block[block_size];
	int bytesreceived = server_connected.Receive(block, block_size);
	LOG("received: ");
	LOG(block);

	server_connected.Send(block, bytesreceived);

	LOG("end processing!");
return 0; }

int main(int argc, char **argv){
	serverfunction();
	return 0;
}
