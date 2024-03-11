// Szkielet klienta TCP/IPv4.
//
// Po podmienieniu SOCK_STREAM na SOCK_DGRAM staje się on szkieletem klienta
// UDP/IPv4 korzystającego z gniazdka działającego w trybie połączeniowym.

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
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
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu

    if (argc != 3) {
        fprintf(stderr, "Użycie: %s adres_ip numer_portu \n", argv[0]);
        return EXIT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = inet_addr(argv[1]) },   // 127.0.0.1 default
        .sin_port = htons(atoi(argv[2]))
    };


    rc = connect(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        return 1;
    }

    unsigned char buf[16];
    

    cnt = read(sock, buf, 16);
    if (cnt == -1) {
        perror("read");
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
