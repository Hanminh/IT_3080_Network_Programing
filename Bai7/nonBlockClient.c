#include "lib.h"

int main() {

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    int ret = connect (client, (struct sockaddr*) &addr, sizeof(addr));

    if(ret < 0) {
        perror("connect");
        return 1;
    }

    unsigned long ul = 1;
    ioctl(client, FIONBIO, &ul);
    ioctl(STDIN_FILENO, FIONBIO, &ul);

    char buf[1024];

    while(1) {
        if(fgets(buf, sizeof(buf), stdin) != NULL) {
            send(client, buf, strlen(buf), 0);
            if(strncmp(buf, "exit", 4) == 0) {
                break;
            }
        }

        int ret = recv(client, buf, sizeof(buf), 0);
        if(ret != -1) {
            if(ret == 0) {
                break;
            }
            buf[ret] = 0;
            printf("Server: %s\n", buf);
        } else {
            if(errno == EWOULDBLOCK) {
                
            } else {
                break;
            }
        }

    }

    close(client);

}