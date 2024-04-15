#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 12345 // to be updated after all the components are ready

class Network {
public:
    Network(); // Constructor for setup
    ~Network(); // Destructor
    std::string receiveData(); // Receives data from the UDP socket.
    
private:
    int sockfd; // Socket file descriptor.
    struct sockaddr_in serverAddr; // Server address structure.
};

#endif 
