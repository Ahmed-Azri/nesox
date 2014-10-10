#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

namespace Nesox {

class TCPSocket {
private:
	int socketfd;
public:
	bool Connect(const char* ip, short port);
	bool Bind(const char* ip, short port);
	bool Listen(int backlog);
	bool Accept(TCPSocket* connected, char* ip, short* port);
	bool Shutdown(int manner);
	bool Close();
	bool setNonBlocking(bool block);
public:
	int Send(char *buffer, int size);
	int Receive(char *buffer, int size);
public:
	TCPSocket();
	~TCPSocket();
public:
	int id(){ return socketfd; }
};

} // namespace nesox

#endif // TCPSOCKET_H
