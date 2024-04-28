#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct SinhVien
{
    char mssv[8];
    char name[40];
    char birth[8];
    double averagePoint;
} sv;

int main(int argc, char *argv[])
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == -1)
    {
        perror("Socket Error");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));

    if (ret == -1)
    {
        perror("Connect fail");
        return 1;
    }

    sv sinhvien;

    while (1)
    {
        memset(&sinhvien, 0, sizeof(sinhvien));

        printf("Ma so sinh vien : ");
        scanf(" %[^\n]", sinhvien.mssv);
        printf("Ho va ten : ");
        scanf(" %[^\n]", sinhvien.name);
        printf("Ngay sinh : ");
        scanf(" %[^\n]", sinhvien.birth);
        printf("Diem trung binh : ");
        scanf(" %lf", &sinhvien.averagePoint);

        send(client, &sinhvien, sizeof(sinhvien), 0);

        if (sinhvien.averagePoint == 0)
        {
            break;
        }
    }
    close(client);
    return 0;
}