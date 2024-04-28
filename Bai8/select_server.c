#include "lib.h"

int main() {
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(listener == -1) {
        printf("Error: Can't create socket\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
    addr.sin_family = AF_INET;

    if(bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("Error: Can't bind socket\n");
        return 1;
    }

    if(listen(listener, 10) == -1) {
        printf("Error: Can't listen\n");
        return 1;
    }

    fd_set fdread;

    int clients[FD_SETSIZE];
    int numClient = 0;

    int maxdp;
    char buf[256];

    while(1) {
        FD_ZERO(&fdread);
        FD_SET(STDIN_FILENO, &fdread);
        FD_SET(listener, &fdread);
        maxdp = listener;

        for(int i = 0 ; i < numClient ; i ++) {
            FD_SET(clients[i], &fdread);
            if(clients[i] > maxdp) {
                maxdp = clients[i];
            }
        }

        int ret = select(maxdp + 1, &fdread, NULL, NULL, NULL);
        if(ret == -1) {
            printf("Error: Can't select\n");
            break;
        }

        if(FD_ISSET(listener, &fdread)) {
            int newClient = accept(listener, NULL, NULL);
            if(newClient > FD_SETSIZE) {
                close(newClient);
            } else {
                clients[numClient++] = newClient;
                printf("New client connected : %d\n", newClient);
            }
        }

        if(FD_ISSET(STDIN_FILENO, &fdread)) {
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = 0;  // remove trailing newline
            for(int i = 0 ; i < numClient ; i++) {
                ret = send(clients[i], buf, strlen(buf), 0);
                if(ret == -1) {
                    perror("Error: Can't send data");
                    break;
                } else {
                    printf("Send: %s\n", buf);
                }
            }
        }

        for(int i = 0 ; i < numClient ; i++) {
            if(FD_ISSET(clients[i], &fdread)) {
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if(ret <= 0) {
                    printf("Socket Closed %d\n", clients[i]);
                    close(clients[i]);

                    if(i < numClient - 1) {
                        clients[i] = clients[numClient - 1];
                    }
                    numClient --;
                    i--;
                    continue;
                } else {
                    buf[ret] = 0;
                    printf("Receive: %s\n", buf);
                }
            }
        }

    }
    return 0 ;
}