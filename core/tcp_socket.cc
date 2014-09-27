#include "tcp_socket.h"
#include "logging.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct sockaddr_in SAI;
typedef struct sockaddr SA;

TCPSocket::TCPSocket() {
  socketid = socket(AF_INET, SOCK_STREAM, 0);
  if (socketid < 0) {
    LOG(FATAL) << "Can't create new socket.";
  }
}

TCPSocket::~TCPSocket() {
  Close();
}

bool TCPSocket::Connect(const char * ip, short port) {
  SAI sa_server;
  sa_server.sin_family      = AF_INET;
  sa_server.sin_port        = htons(port);

  if (inet_pton(AF_INET, ip, &sa_server.sin_addr) > 0 && connect(socketid, reinterpret_cast<SA*>(&sa_server), sizeof(sa_server)) >= 0) {
    return true;
  }

  LOG(ERROR) << "Failed connect to " << ip << ":" << port;
  return false;
}

bool TCPSocket::Bind(const char * ip, short port) {
  SAI sa_server;
  sa_server.sin_family      = AF_INET;
  sa_server.sin_port        = htons(port);

  if (inet_pton(AF_INET, ip, &sa_server.sin_addr) > 0 && bind(socketid, reinterpret_cast<SA*>(&sa_server), sizeof(sa_server)) >= 0) {
    return true;
  }

  LOG(ERROR) << "Failed bind on " << ip << ":" << port;
  return false;
}

bool TCPSocket::Listen(int max_connection) {
  if (listen(socketid, max_connection) >= 0) {
    return true;
  }

  LOG(ERROR) << "Failed listen on socket fd: " << socketid;
  return false;
}

bool TCPSocket::Accept(TCPSocket * socket, std::string * ip, short * port) {
  int sock_client;
  SAI sa_client;
  socklen_t len = sizeof(sa_client);

  sock_client = accept(socketid, reinterpret_cast<SA*>(&sa_client), &len);
  if (sock_client < 0) {
    LOG(ERROR) << "Failed accept connection on " << ip << ":" << port;
    return false;
  }

  char tmp[INET_ADDRSTRLEN];
  const char * ip_client = inet_ntop(AF_INET, &sa_client.sin_addr, tmp, sizeof(tmp));
  if (ip_client == NULL) return false;
  ip->assign(ip_client);
  *port = ntohs(sa_client.sin_port);
  socket->socketid = sock_client;

  return true;
}

bool TCPSocket::SetBlocking(bool flag) {
  int opts;

  if ((opts = fcntl(socketid, F_GETFL)) < 0) {
    LOG(ERROR) << "Failed to get socket status.";
    return false;
  }

  if (flag) {
    opts |= O_NONBLOCK;
  } else {
    opts &= ~O_NONBLOCK;
  }

  if (fcntl(socketid, F_SETFL, opts) < 0) {
    LOG(ERROR) << "Failed to set socket status.";
    return false;
  }

  return true;
}

bool TCPSocket::ShutDown(int ways) {
  return 0 == shutdown(socketid, ways);
}

void TCPSocket::Close() {
  if (socketid >= 0) {
	close(socketid);
    socketid = -1;
  }
}

int TCPSocket::Send(const char * data, int len_data) {
  return send(socketid, data, len_data, 0);
}

int TCPSocket::Receive(char * buffer, int size_buffer) {
  return recv(socketid, buffer, size_buffer, 0);
}

int TCPSocket::Socket() const {
  return socketid;
}

