#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

int main() {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));

    if(ret == -1) {
        perror("Connet() Error");
        return 1;
    }

    char buf[256];

    while(1) {
        printf("Nhap chuoi ki tu : ");
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, strlen(buf), 0);

        if (strncmp(buf, "exit", 4) == 0)
            break;

        printf("Dang cho du lieu tu server ...\n");
        int ret = recv(client, buf, sizeof(buf), 0);
        if(ret < 0) {
            break;
        }
        buf[ret] = 0;
        printf("Server : %s\n", buf);
    }

    close(client);
    return 0 ;
}