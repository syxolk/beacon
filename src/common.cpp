#include "beacon/common.h"

#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <cstring> // strerror
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h> // close

namespace beacon {

BeaconException::BeaconException(const std::string &message) : message(message) {
}

const char* BeaconException::what() const noexcept {
    return message.c_str();
}

void UdpSocket::bind(const std::string &address, std::uint16_t port) {
    struct sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    if(0 == inet_aton(address.c_str(), &servAddr.sin_addr)) {
        throw BeaconException(std::string("Failed to call inet_aton(): ") + std::strerror(errno));
    }
    servAddr.sin_port = htons(port);
    if(-1 == ::bind(fd, (struct sockaddr *) &servAddr, sizeof(servAddr))) {
        throw BeaconException(std::string("Failed to call bind(): ") + std::strerror(errno));
    }
}

void UdpSocket::bind(std::uint16_t port) {
    bind("0.0.0.0", port);
}

void UdpSocket::bind(const std::string &address) {
    bind(address, 0);
}

void UdpSocket::bind() {
    bind("0.0.0.0", 0);
}

UdpSocket::UdpSocket() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(-1 == fd) {
        throw BeaconException(std::string("Failed to call socket(): ") + std::strerror(errno));
    }
}

UdpSocket::~UdpSocket() {
    close(fd);
}

void UdpSocket::enableBroadcast(bool enable) {
    int broadcastEnable = enable;
    int ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable,
                       sizeof(broadcastEnable));

    if(ret == -1) {
        throw BeaconException(std::string("Failed to call setsockopt(): ") + std::strerror(errno));
    }
}

void UdpSocket::enableReuseAddr(bool enable) {
    int reuseaddrEnable = enable;
    int ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &reuseaddrEnable,
                       sizeof(reuseaddrEnable));

    if(ret == -1) {
        throw BeaconException(std::string("Failed to call setsockopt(): ") + std::strerror(errno));
    }
}

void UdpSocket::send(const Datagram &datagram) {
    struct sockaddr_in s;
    std::memset(&s, 0, sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = htons(datagram.port);
    if(0 == inet_aton(datagram.address.c_str(), &s.sin_addr)) {
        throw BeaconException(std::string("Failed to call inet_aton(): ") + std::strerror(errno));
    }
    int res = sendto(fd, datagram.buffer, datagram.bufferUsed, 0, (struct sockaddr *)&s,
                     sizeof(struct sockaddr_in));

    if(res < 0) {
        throw BeaconException(std::string("Failed to call sendto(): ") + std::strerror(errno));
    }
}

bool UdpSocket::receive(Datagram &datagram) {
    struct sockaddr_in clientAddr;
    unsigned int clientAddrLen = sizeof (clientAddr);

    size_t received = recvfrom(fd, datagram.buffer, Datagram::BUF_SIZE, 0,
                               (struct sockaddr *) &clientAddr, &clientAddrLen);

    if(received > 0) {
        datagram.bufferUsed = received;
        datagram.address = inet_ntoa(clientAddr.sin_addr);
        datagram.port = clientAddr.sin_port;
        return true;
    } else {
        return false;
    }
}

bool UdpSocket::receiveNonblock(Datagram &datagram) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int retval = select(fd + 1, &rfds, NULL, NULL, &tv);

    if(retval > 0 && FD_ISSET(fd, &rfds)) {
        return receive(datagram);
    } else {
        return false;
    }
}

} // namespace beacon
