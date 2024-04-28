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

    fd_set fdread, fdtest;
    FD_ZERO(&fdread);
    FD_SET(listener, &fdread);
    FD_SET(STDIN_FILENO, &fdread);

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    char buf[256];
    int client_socket[FD_SETSIZE];
    int numClient = 0;

    while(1) {
        fdtest = fdread;
        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, &tv);
        if(ret == -1) {
            printf("Error: Can't select\n");
            break;
        }

        for(int i = 0; i < FD_SETSIZE; i ++) {
            if(FD_ISSET(i,&fdtest)) {
                if(i == listener) {
                    int client = accept(listener, NULL, NULL);
                    if(client >= FD_SETSIZE) {
                        close(client);
                    } else {
                        FD_SET(client, &fdread);
                        printf("New client connected: %d\n", client);
                    }
                } else {
                    int client = i ;
                    ret = recv(client, buf, sizeof(buf), 0);
                    if(ret <=0) {
                        close(client);
                        FD_CLR(client, &fdread);
                        printf("Client disconnected: %d\n", client);
                    }
                    buf[ret] = 0;
                    printf("Received from %d: %s\n", client, buf);

                    int j = 0 ;
                    for(; j < numClient ; j ++) {
                        if(client_socket[j] == client) {
                            break;
                        }
                    }

                    //Not Login
                    if(j == numClient){
                        char user[32], pass[32], tmp[65], line[65];
                        int n = sscanf(buf, "%s %s %s", user, pass, tmp);
                        if(n !=2) {
                            char *msg = "Invalid command\n";
                            send(client, msg, strlen(msg), 0);
                        } else {
                            sprintf(tmp, "%s %s\n", user, pass);
                            FILE *f = fopen("account.txt", "r");
                            int found = 0;
                            while(fgets(line, sizeof(line), f)) {
                                if(strcmp(line, tmp) == 0) {
                                    found = 1;
                                    break;
                                }
                            }
                            if(found) {
                                char *msg = "Login success\n";
                                send(client, msg, strlen(msg), 0);

                                client_socket[numClient++] = client;
                                
                            } else {
                                char *msg = "Login failed\n";
                                send(client, msg, strlen(msg), 0);
                            }
                            fclose(f);
                        }
                    } else {
                        //Logined
                        char cmd[300];
                        if(buf[strlen(buf) - 1] == '\n') {
                            buf[strlen(buf) - 1] = 0;
                        }

                        sprintf(cmd, "%s > tmp.txt", buf);
                        system(cmd);

                        FILE *f = fopen("tmp.txt", "r");
                        while(1) {
                            int n = fread(buf, 1, sizeof(buf), f);
                            if(n <= 0) {
                                break;
                            }
                            send(client, buf, n, 0);
                        }
                        fclose(f);
                    }
                }
            }
        }

    }
    return 0;
}