#include "tcpsocket.h"

Nesox::TCPSocket::TCPSocket()
{
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

Nesox::TCPSocket::~TCPSocket()
{
	Close();
}

bool Nesox::TCPSocket::Close()
{
	if (close(socketfd) < 0) return false;
	return true;
}

bool Nesox::TCPSocket::Shutdown(int manner) //manner: SHUT_RD; SHUT_WR; SHUT_RDWR;
{
	if (shutdown(socketfd, manner) < 0) return false;
	return true;
}

bool Nesox::TCPSocket::Connect(const char *ip, short port)
{
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	inet_pton(sa.sin_family, ip, &sa.sin_addr);
	sa.sin_port = htons(port);
  	connect(socketfd, reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)));
  	return true;
}

bool Nesox::TCPSocket::Bind(const char * ip, short port)
{
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	inet_pton(sa.sin_family, ip, &sa.sin_addr);
	sa.sin_port = htons(port);
  	bind(socketfd, reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)));
  	return true;
}

bool Nesox::TCPSocket::Listen(int backlog)
{
	if (listen(socketfd, backlog) < 0) return false;
	return true;
}

bool Nesox::TCPSocket::Accept(Nesox::TCPSocket *connected, char *ip, short *port)
{
	int connectfd;
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);
	connectfd = accept(socketfd, reinterpret_cast<struct sockaddr *>(&sa), &len);
	if (connectfd < 0) return false;

	char ipstore[INET_ADDRSTRLEN];
	const char *p = inet_ntop(AF_INET, &sa.sin_addr, ipstore, sizeof(ipstore));
	if (p == NULL) return false;
	snprintf(ip, INET_ADDRSTRLEN, "%s", p);
	*port = static_cast<short> ntohs(sa.sin_port);
	connected->socketfd = connectfd;
	return true;
}

bool Nesox::TCPSocket::setNonBlocking(bool block)
{
	int options;
	if ((options = fcntl(socketfd, F_GETFL)) < 0) return false;
	options = (block) ? options | O_NONBLOCK : options & ~O_NONBLOCK;
	if (fcntl(socketfd, F_SETFL, options) < 0) return false;
	return true;
}

int Nesox::TCPSocket::Send(char *buffer, int size)
{
	return send(socketfd, buffer, size, 0);
}

int Nesox::TCPSocket::Receive(char *buffer, int size)
{
	return recv(socketfd, buffer, size, 0);
}
