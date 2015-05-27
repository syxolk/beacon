# beacon

## Installation
- Clone repository
- Create build folder
- Run cmake in build folder
- Run make in build folder

## Usage
````sh
# The sender is broadcasting a packet on port 7284 every second with the given message
./beacon --sender <message>

# The receiver waits for UDP packets on port 7284
./beacon --receiver
````
