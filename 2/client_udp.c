// Szkielet klienta UDP/IPv4 używającego gniazdka bezpołączeniowego.

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


bool printable_buf(const void *buf, int len) {
    const unsigned char *p = (const unsigned char *) buf;
    const unsigned char *end = p + len;
    for (; p != end ; ++p) {
        if (*p == '\n' || *p == '\r' || *p == '\t') 
            continue;

        if (*p < 32 || *p > 126)
            return false;  
    }

    return true; 
}


int main(int argc, char *argv[])
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()

    if (argc != 3) {
        fprintf(stderr, "Użycie: %s adres_ip numer_portu \n", argv[0]);
        return EXIT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = inet_addr(argv[1]) },   
        .sin_port = htons(atoi(argv[2]))
    };

    if (addr.sin_addr.s_addr == -1) {
        perror("Invalid ip address");
        return 1;
    }

    unsigned char buf[32];
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
     

    cnt = sendto(sock, buf, 0, 0, (struct sockaddr *) & addr, sizeof(addr));
    if (cnt == -1) {
        perror("sendto");
        return 1;
    }

    cnt = recvfrom(sock, buf, 16, 0, NULL, NULL);
    if (cnt == -1) {
        perror("recvfrom");
        return 1;
    }
    if (printable_buf(buf, cnt)) {
        fwrite(buf, sizeof(char), cnt, stdout);
    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}