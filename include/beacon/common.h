#ifndef BEACON_COMMON_H_
#define BEACON_COMMON_H_

#include <string>
#include <exception>

namespace beacon {

class BeaconException : public std::exception {
public:
    BeaconException(const std::string &message);
    virtual const char* what() const noexcept override;
private:
    std::string message;
};

struct Datagram {
    static constexpr size_t BUF_SIZE = 1024;

    std::string address;
    std::uint16_t port;
    char buffer[BUF_SIZE];
    size_t bufferUsed;
};

class UdpSocket {
public:
    UdpSocket(const std::string &address, std::uint16_t port);
    explicit UdpSocket(std::uint16_t port);
    explicit UdpSocket(const std::string &address);
    UdpSocket();
    ~UdpSocket();

    void enableBroadcast(bool enable = true);

    void send(const Datagram &datagram);
    bool receive(Datagram &datagram);
    bool receiveNonblock(Datagram &datagram);
private:
    void bind(const std::string &address, std::uint16_t port);

    int fd;
};

} // namespace beacon

#endif // BEACON_COMMON_H_
