#include "beacon/sender.h"
#include "beacon/receiver.h"

#include <cstdlib>
#include <cstring>

const int PORT = 7284;

int main(int argc, char *argv[]) {
    if(argc >= 2) {
        if(strcmp(argv[1], "--receiver") == 0) {
            beacon::receiver(PORT);
        } else if(strcmp(argv[1], "--sender") == 0) {
            beacon::sender(PORT, argv[2]);
        } else {
            printf("Invalid option\n");
        }
    } else {
        printf("Wrong number of options\n");
    }

    return EXIT_SUCCESS;
}
