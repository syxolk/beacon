#ifndef BEACON_DAEMON_H_
#define BEACON_DAEMON_H_

namespace beacon {

/**
 * @brief Daemonizes the process.
 *
 * Described in
 * http://www.linuxprofilm.com/articles/linux-daemon-howto.html
 * http://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux
 */
void daemonize();

} // namespace beacon

#endif // BEACON_DAEMON_H_
