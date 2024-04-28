#include "lib.h"

int main() {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client == -1) {
        printf("Error: Can't create socket\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);
    addr.sin_family = AF_INET;

    int ret = connect(client, (struct sockaddr*)&addr, sizeof(addr));

    if(ret == -1) {
        printf("Error: Can't connect to server\n");
        return 1;
    }

    fd_set fdread ;
    FD_ZERO(&fdread);

    FD_SET(STDIN_FILENO, &fdread);
    FD_SET(client, &fdread);

    fd_set fdtest;

    struct timeval tv ;

    char buf[256];
    while(1) {
        fdtest = fdread;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int ret = select(client + 1, &fdtest, NULL, NULL, &tv);

        if(ret == -1) {
            printf("Error: Can't select\n");
            break;
        }

        if(ret == 0) {
            printf("Timeout\n");
            continue;
        }

        if(FD_ISSET(STDIN_FILENO, &fdtest)) {
            fgets(buf, sizeof(buf), stdin);
            ret = send(client, buf, strlen(buf), 0);
            printf("Send: %s\n", buf);
            printf("Byte Send = %d\n", ret);
            if(strncmp(buf, "exit", 4) == 0) {
                break;
            }
        }

        if(FD_ISSET(client, &fdtest)) {
            ret = recv(client, buf, sizeof(buf), 0);
            if(ret <= 0) {
                printf("Error: Can't receive data\n");
                break;
            }
            buf[ret] = 0;
            printf("Receive: %s\n", buf);
        }

    }
    close(client);

}