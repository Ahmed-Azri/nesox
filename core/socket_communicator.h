#ifndef _SOCKET_COMMUNICATOR_H_
#define _SOCKET_COMMUNICATOR_H_


#include "communicator.h"
#include "tcp_socket.h"
#include "logging.h"

#include "boost/thread.hpp"

class SocketCommunicator : public Communicator {
private:

	bool is_sender_;
	int num_sender_;
	int num_receiver_;
	int worker_id_;
	int map_queue_size_;
	int reduce_queue_size_;

	std::vector<TCPSocket*> sockets_;
	std::vector<SignalingQueue*> send_buffers_;
	scoped_ptr<SignalingQueue> receive_buffer_;

	std::map<int, int> socket_id_;

	scoped_ptr<boost::thread> thread_send_;
	scoped_ptr<boost::thread> thread_receive_;

	bool InitSender(const std::vector<std::string> &reducers);
	bool InitReceiver(const std::string &reducer);
	bool FinalizeSender();
	bool FinalizeReceiver();

	static void SendLoop(SocketCommunicator *pcom);
	static void ReceiveLoop(SocketCommunicator *pcom);

public:
	virtual bool Initialize(bool is_map_worker, int num_map_workers,
	                        const std::vector<std::string> &reduce_workers,
	                        int map_queue_size, int reduce_queue_size,
	                        int worker_id);

	virtual int Send(void *src, int size, int receiver_id);
	virtual int Send(const std::string &src, int receiver_id);

	virtual int Receive(void *dest, int max_size);
	virtual int Receive(std::string *dest);

	virtual bool Finalize();

public:
	SocketCommunicator() {}
	virtual ~SocketCommunicator() {}
};


#endif /*_SOCKET_COMMUNICATOR_H_*/