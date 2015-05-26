#include "beacon/receiver.h"
#include "beacon/common.h"

#include <iostream>
#include <unistd.h>

namespace beacon {

void receiver(std::uint16_t port) {
    UdpSocket socket;
    socket.enableReuseAddr();
    socket.bind(port);

    Datagram datagram;

    while(true) {
        if(socket.receiveNonblock(datagram)) {
            std::cout << datagram.address << " " << datagram.port <<
                         " " << datagram.buffer << std::endl;
        } else {
            // only sleep if nothing was received
            usleep(1000*1000);
        }
    }
}

} // namespace beacon
