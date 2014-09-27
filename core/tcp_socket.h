#ifndef _TCP_SOCKET_
#define _TCP_SOCKET_

#include <sys/socket.h>
#include <string>

class TCPSocket {
private:
	int socketid;
public:

	bool Connect(const char * ip, short port);
	bool Bind(const char * ip, short port);
	bool Listen(int max_connection);
	bool Accept(TCPSocket * socket, std::string * ip_client, short * port_client);

	bool SetBlocking(bool flag);

	bool ShutDown(int ways);
	void Close();

	int Send(const char * data, int len_data);
	int Receive(char * buffer, int size_buffer);

	int Socket() const;
public:
	TCPSocket();
	~TCPSocket();
};

#endif /*_TCP_SOCKET_*/
