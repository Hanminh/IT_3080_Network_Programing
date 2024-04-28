#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( client == -1) {
        perror("Socket() error");
        return 1 ;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[3]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));

    if(ret == -1) {
        perror("Connet() Error");
        return 1;
    }

    char buffer[100];
    memset(buffer, 97, sizeof(buffer));

    while(1) {
        char message[1000];
        fgets(message, sizeof(message),stdin);
        int byteSend = send(client, message, strlen(message), 0);
        if(byteSend <= 0) {
            break;
        }
        printf("Number of sent bytes : %d\n", byteSend);
    }
    close(client);
}