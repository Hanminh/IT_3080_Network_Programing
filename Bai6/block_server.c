#include "lib.h"

int main() {
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1) {
        perror("Socket() error");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
    addr.sin_family = AF_INET;

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("Bind() error");
        return 1;
    }

    if(listen(listener, 5)) {
        perror("Listen() error");
        return 1;
    }

    int clients[64];
    int numClients = 0;
    char buf[256];

    while(1) {
        printf("Waiting for new client ...\n");
        int client = accept(listener, NULL, NULL);

        clients[numClients] = client;
        numClients ++ ;

        printf("New client connected : %d\n", client);
        for(int i = 0; i < numClients; i ++) {
            int ret = recv(clients[i], buf, sizeof(buf), 0);
            if(ret <=0) {
                continue;
            }
            buf[ret] = 0;
            printf("Client %d : %s\n", clients[i], buf);
        }

    }

    close(listener);   

}