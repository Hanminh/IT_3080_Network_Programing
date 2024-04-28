#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<dirent.h>

int main() {
    DIR *d;
    struct dirent *dir;
    char folder[] = "/home/hanminh/projects/helloworld/Bai5";
    d = opendir(folder);

    if(d) {
        while((dir = readdir(d)) != NULL) {
            printf("%s", dir -> d_name);
        }
    }
}