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
    int reuse = 1;

    if (fd >= 0) {
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) >= 0 &&
            setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) >= 0) {
            struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_addr.s_addr = htonl(INADDR_ANY),
                .sin_port = htons(MULTICAST_PORT)
            };
            socklen_t addrlen = sizeof(addr);

            if (bind(fd, (struct sockaddr *) &addr, addrlen) >= 0) {
                struct ip_mreq mreq = {
                    .imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR),
                    .imr_interface.s_addr = htonl(INADDR_ANY)
                };

                if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) >= 0) {
                    char buf[BUFSIZ];
                    ssize_t nbytes;

                    while (1) {
                        addrlen = sizeof(addr);

                        if ((nbytes = recvfrom(fd, buf, BUFSIZ, 0, (struct sockaddr *) &addr, &addrlen)) >= 0) {
                            printf("%s", buf);
                        }
                        else {
                            perror("recvfrom");
                            exit_status = EXIT_FAILURE;
                            break;
                        }
                    }
                }
                else {
                    perror("setsockopt");
                    exit_status = EXIT_FAILURE;
                }
            }
            else {
                perror("bind");
                exit_status = EXIT_FAILURE;
            }
        }
        else {
            perror("setsockopt");
            exit_status = EXIT_FAILURE;
        }
    }
    else {
        perror("socket");
        exit_status = EXIT_FAILURE;
    }

    return exit_status;
}
