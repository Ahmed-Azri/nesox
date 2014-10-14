#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

#include <map>
#include <string>
#include <vector>

class Communicator {
public:
    virtual bool Initialize(bool is_map_worker, int num_map_workers,
                            const std::vector<std::string> &reduce_workers,
                            int map_queue_size, int reduce_queue_size,
                            int worker_id) = 0;

    virtual int Send(void *src, int size, int receive_id) = 0;
    virtual int Send(const std::string &src, int receive_id) = 0;

    virtual int Receive(void *dest, int max_size) = 0;
    virtual int Receive(std::string *dest) = 0;

    virtual bool Finalize() = 0;
public:
    virtual ~Communicator() {}
};


#endif /*_COMMUNICATOR_H_*/
