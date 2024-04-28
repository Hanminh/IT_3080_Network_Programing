#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <log_file>\n", argv[0]);
        return 1;
    }

    char *log_file = argv[2];

    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server, (struct sockaddr *)&addr, sizeof(addr));
    listen(server, 5);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client = accept(server, (struct sockaddr *)&client_addr, &client_len);

        char buffer[1024];
        int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);
        buffer[bytes] = '\0';

        time_t now = time(NULL);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

        printf("%s %s %s\n", inet_ntoa(client_addr.sin_addr), time_str, buffer);

        FILE *log = fopen(log_file, "a");
        if (log != NULL) {
            int a = fprintf(log, "%s %s %s\n", inet_ntoa(client_addr.sin_addr), time_str, buffer);
            fclose(log);
        }

        close(client);
    }

    close(server);
    return 0;
}