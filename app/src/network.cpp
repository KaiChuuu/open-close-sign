#include "network.h"
#include <iostream>
#include <cstring> // for memset and stuff
#include <unistd.h> // for close()
#include <arpa/inet.h> // net functions
#include <sys/socket.h> // for socket

// make udp socket
Network::Network() {
    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket no work" << std::endl;
        exit(1); // exit if fail
    }

    // make server address
    memset(&serverAddr, 0, sizeof(serverAddr)); // clear struct
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(SERVER_PORT); // port
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // any ip

    // bind socket to address
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "bind fail" << std::endl;
        exit(1); // exit if bind fail
    }
}

// close socket when done
Network::~Network() {
    close(sockfd); // close it
}

// wait for data
std::string Network::receiveData() {
    char buffer[1024] = {0}; // where data goes
    struct sockaddr_in clientAddr; // client address
    socklen_t len = sizeof(clientAddr); // size of address

    // wait for message
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &len);
    if (n < 0) {
        std::cerr << "recv fail" << std::endl;
        return ""; // return empty if fail
    }

    // convert buffer to string and give back
    return std::string(buffer, n);
}
