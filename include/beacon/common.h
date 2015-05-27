#ifndef BEACON_COMMON_H_
#define BEACON_COMMON_H_

#include <string>
#include <exception>

namespace beacon {

/**
 * @brief Exception that can be thrown by all UdpSocket methods and
 * costructors.
 */
class BeaconException : public std::exception {
public:
    /**
     * @brief Initialize a beacon exception with a message.
     * @param message exception message, will be returned by what()
     */
    BeaconException(const std::string &message);

    /**
     * @brief Returns the message given in the constructor.
     * @return exception message
     */
    virtual const char* what() const noexcept override;
private:
    std::string message;
};

/**
 * @brief UDP datagram with buffer and source/destination address and port.
 */
struct Datagram {
    static constexpr size_t BUF_SIZE = 1024;

    /**
     * @brief IPV4 address
     */
    std::string address;

    /**
     * @brief UDP port
     */
    std::uint16_t port;

    /**
     * @brief char buffer with UDP payload
     */
    char buffer[BUF_SIZE];

    /**
     * @brief Number of bytes that are used of the buffer
     */
    size_t bufferUsed;
};

/**
 * @brief Abstraction for a UDP socket.
 */
class UdpSocket {
public:
    /**
     * @brief Create a new datagram socket. The socket is not binded.
     */
    UdpSocket();

    /**
     * @brief Closes the underlying socket.
     */
    ~UdpSocket();

    /**
     * @brief Bind the socket to a local address and a local port.
     * @param address IPV4 address
     * @param port UDP port
     */
    void bind(const std::string &address, std::uint16_t port);

    /**
     * @brief Bind the socket to a local port and any IP address.
     *
     * This is useful for listening on a specific port.
     *
     * @param port local UDP port
     */
    void bind(std::uint16_t port);

    /**
     * @brief Bind the socket to a local address and any port.
     * @param address local IPV4 address
     */
    void bind(const std::string &address);

    /**
     * @brief Bind the socket to any address and any port.
     */
    void bind();

    /**
     * @brief Enable or disable broadcasting. This is needed if socket should
     * send packages to a broadcast address.
     * @param enable true for enable, false for disable
     */
    void enableBroadcast(bool enable = true);

    /**
     * @brief Enable or disable address reusing. Sockets that are in TIME_WAIT
     * state can be reused immediately if enabled.
     * @param enable true enable, false for disable
     */
    void enableReuseAddr(bool enable = true);

    /**
     * @brief Send a UDP datagram to the destination address and port that are
     * set in the datagram
     * @param datagram UDP packet to send
     */
    void send(const Datagram &datagram);

    /**
     * @brief Wait until a UDP packet is received and store the received bytes
     * in the buffer. Address and port of the packet's sender is stored in
     * the given datagram.
     * @param datagram UDP packet to receive
     * @return false if the socket returned an error, true otherwise
     */
    bool receive(Datagram &datagram);

    /**
     * @brief Do not wait for UDP packets but instead look if there are
     * available packets and return the packet immediately.
     * @param datagram UDP paclet to receive
     * @return true if a packet is available and no errors occured, otherwise
     * false
     */
    bool receiveNonblock(Datagram &datagram);
private:
    int fd;
};

} // namespace beacon

#endif // BEACON_COMMON_H_
