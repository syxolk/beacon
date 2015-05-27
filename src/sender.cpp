#include "beacon/sender.h"
#include "beacon/common.h"

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <linux/if.h>

namespace beacon {

void sender(std::uint16_t port, const std::string &message, bool enableLogging) {
    std::vector<std::string> addresses;
    getIPV4BroadcastAddresses(addresses);

    if(enableLogging) {
        for(std::string a : addresses) {
            std::cout << a << std::endl;
        }
    }

    UdpSocket socket;
    socket.enableBroadcast();
    socket.enableReuseAddr();
    socket.bind();
    Datagram datagram;

    datagram.port = port;
    std::strcpy(datagram.buffer, message.c_str());
    datagram.bufferUsed = message.size() + 1;

    while(true) {
        for(const std::string address : addresses) {
            datagram.address = address;
            socket.send(datagram);
        }

        usleep(1000*1000);
    }
}

bool getIPV4BroadcastAddresses(std::vector<std::string> &result) {
   struct ifaddrs *ifaddr, *ifa;
   int family, s;
   char host[NI_MAXHOST];

   if (getifaddrs(&ifaddr) == -1) {
       perror("Failed to call getifaddrs");
       return false;
   }

   /* Walk through linked list, maintaining head pointer so we
      can free list later */

   for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
       if (ifa->ifa_addr == NULL)
           continue;

       family = ifa->ifa_addr->sa_family;

       /* For an AF_INET* interface address, display the address */

       if (family == AF_INET) {
           if(ifa->ifa_ifu.ifu_broadaddr != NULL &&
                   ifa->ifa_ifu.ifu_broadaddr->sa_family == AF_INET &&
                   (ifa->ifa_flags & IFF_BROADCAST)) {
               s = getnameinfo(ifa->ifa_ifu.ifu_broadaddr,
                       (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                             sizeof(struct sockaddr_in6),
                       host, NI_MAXHOST,
                       NULL, 0, NI_NUMERICHOST);
               if (s != 0) {
                   printf("getnameinfo() failed: %s\n", gai_strerror(s));
                   continue;
               }

               result.push_back(host);
           }
       }
   }

   freeifaddrs(ifaddr);

   return true;
}

} // namespace beacon
