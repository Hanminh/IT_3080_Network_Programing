#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{   
    printf("%s", argv[3]);

    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listener == -1)
    {
        perror("Listen fail");
        return 1;
    }

    // Server's address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("Bind Error");
        return 1;
    }

    if (listen(listener, 5))
    {
        perror("Listen Error");
        return 1;
    }

    printf("Connecting ...\n");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buf[256];
    memset(buf, 97, sizeof(buf));
    while (1)
    {
        int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
        printf("New Client Access : %d - IP : %s - Port : %d\n", client, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        FILE *sayHi = fopen(argv[2], "r");
        char line[256];
        while (fgets(line, sizeof(line), sayHi))
        {
            int sd = send(client, line, strlen(line), 0);
        }
        fclose(sayHi);
        while (1)
        {
            FILE *recvFile = fopen("recvFile.txt", "a");
            memset(buf, 97, sizeof(buf));
            int ret = recv(client, buf, sizeof(buf), 0);
            if (ret <= 0)
            {
                break;
            }
            buf[ret] = 0;
            int a = fprintf(recvFile, "%s", buf);
            printf("Received Data : %s", buf);
            fclose(recvFile);
        }
        close(client);
    }

    close(listener);
    return 0;
}

