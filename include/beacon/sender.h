#ifndef BEACON_SENDER_H_
#define BEACON_SENDER_H_

#include <vector>
#include <string>

namespace beacon {

void sender(std::uint16_t port, const std::string &message);
bool getIPV4BroadcastAddresses(std::vector<std::string> &result);

} // namespace beacon

#endif // BEACON_SENDER_H_
