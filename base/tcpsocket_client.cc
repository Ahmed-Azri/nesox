#include "tcpsocket.h"
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>

#define LOG(message) log(message)

inline int log(char* message) { return std::cout << std::string(message) << std::endl; }

static const int block_size = 0xffff;

int clientfunction();

int clientfunction(){
	sleep(2);
	LOG("client: start");
	Nesox::TCPSocket client;

	LOG("client: connecting");
	client.Connect("127.0.0.1", 6655);
	LOG("client: connected");

	char block[block_size] = "From Client to Server: Hello, Nesox!";
	client.Send(block, std::strlen(block));
	client.Receive(block, block_size);
	LOG(block);

	LOG("client: end");
return 0; }

int main(int argc, char **argv){
	clientfunction();
}
