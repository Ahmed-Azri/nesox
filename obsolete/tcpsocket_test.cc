#include "tcpsocket.h"
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>

#define LOG(message) log(message)

inline int log(char* message) { return std::cout << std::string(message) << std::endl; }

static const int block_size = 0xffff;

int serverfunction();
int clientfunction();

int serverfunction(){
	LOG("server: start processing!");

	Nesox::TCPSocket server_listening;
	Nesox::TCPSocket server_connected;

	server_listening.Bind("127.0.0.1", 8484);

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


int clientfunction(){
	sleep(2);
	LOG("client: start");
	Nesox::TCPSocket client;

	LOG("client: connecting");
	client.Connect("127.0.0.1", 8484);
	LOG("client: connected");

	char block[block_size] = "From Client to Server: Hello, Nesox!";
	client.Send(block, std::strlen(block));
	client.Receive(block, block_size);
	LOG(block);

	LOG("client: end");
return 0; }

int main(int argc, char **argv){
	int pid = fork();
	if (pid > 0) {
		LOG("parent: ");
		std::cout << pid << std::endl;
		serverfunction();
		return pid;
	}
	else {
		LOG("child: ");
		std::cout << pid << std::endl;
		clientfunction();
		return pid;
	}
	return 0;
}
