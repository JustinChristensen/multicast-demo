#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "constants.h"

int main(int argc, char const *argv[])
{
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    int exit_status = EXIT_SUCCESS;

    if (fd >= 0) {
        struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = inet_addr(MULTICAST_ADDR),
            .sin_port = htons(MULTICAST_PORT)
        };
        char buf[BUFSIZ];

        while (fgets(buf, sizeof buf, stdin) != NULL) {
            if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                perror("sendto");
                exit_status = EXIT_FAILURE;
                break;
            }
        }
    }
    else {
        perror("socket");
        exit_status = EXIT_FAILURE;
    }

    return exit_status;
}
