#include "lib.h"

int main() {
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener < 0) {
        perror("socket");
        return 1;
    }

    unsigned long ul = 1;
    ioctl(listener, FIONBIO, &ul);

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    if(bind(listener, (struct sockaddr*) &addr , sizeof(addr)) ) {
        perror("bind");
        return 1;
    }

    if (listen(listener, 5)) {
        perror("listen() failed");
        return 1;
    }


    int clients[64];
    int numClient = 0;

    char buf[1024];

    while(1) {
        int client = accept(listener, NULL, NULL);
        if (client != -1) 
        {
            printf("Co ket noi moi: %d\n", client);

            // Chuyen socket sang che do bat dong bo
            ul = 1;
            ioctl(client, FIONBIO, &ul);

            clients[numClient] = client;
            numClient++;
        }
        else {
            if (errno == EWOULDBLOCK) {
                // Loi do chua co ket noi
                // Khong xu ly gi them
            } else {
                break;
            }
        }

         for (int i = 0; i < numClient; i++)
        {
            int ret = recv(clients[i], buf, sizeof(buf), 0);
            if (ret != -1) {
                if (ret == 0)
                    continue;

                buf[ret] = 0;
                printf("Received from %d: %s\n", clients[i], buf);
            } else {
                if (errno == EWOULDBLOCK) {
                    // Loi do chua co du lieu
                    // Khong xu ly gi them
                } else {
                    continue;
                }
            }
        }
    }

    close(listener);
    return 1;
}