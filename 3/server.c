// Szkielet serwera UDP/IPv4 używającego gniazdka bezpołączeniowego.

#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define MAX_PACKET_SIZE 65535
#define MAX_RESPONSE_SIZE 1024


bool is_letters_eq(unsigned char *p, unsigned char *q) {
    if (*p > 64 && *p < 91) {
        return (*p == *q) || ((*p) + 32 == *q);
    }
    else if (*p > 96 && *p < 123) {
        return (*p == *q) || ((*p) - 32 == *q);
    }
    return false;
}

bool process_str(unsigned char *str_start, unsigned char *str_end) {
    unsigned char *p = str_start;
    unsigned char *q = str_end;

    for (;  p < q; ++p, --q) {
        if (is_letters_eq(p, q)) {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

ssize_t process_buf(unsigned char buf[], int len, char response[]) {
    unsigned char *p = buf;
    unsigned char *end = buf + len;

    while( p != end && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t')) {
        ++p;
    }

    unsigned char *str_start = p;
    unsigned char *str_end = NULL;
    unsigned int words_counter = 0;
    unsigned int palindroms_counter = 0;

    for (; p != end; ++p) {
        if (*p == ' ' || p + 1 == end) {
            str_end = p - 1;
            if (process_str(str_start, str_end)) {
                ++palindroms_counter;
            }
            ++words_counter;
            str_start = p + 1;
        }
    }

    ssize_t response_len = snprintf(response, MAX_RESPONSE_SIZE, "%d/%d", palindroms_counter, words_counter);
    return response_len;
}


int main(int argc, char *argv[])
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()
    ssize_t response_len = 0;

    if (argc != 2) {
        fprintf(stderr, "Użycie: %s numer_portu \n", argv[0]);
        return EXIT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(atoi(argv[1]))
    };

    rc = bind(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    bool keep_on_handling_clients = true;
    while (keep_on_handling_clients) {

        unsigned char buf[MAX_RESPONSE_SIZE];
        char response[MAX_RESPONSE_SIZE];

        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len;

        clnt_addr_len = sizeof(clnt_addr);
        cnt = recvfrom(sock, buf, MAX_RESPONSE_SIZE, 0,
                (struct sockaddr *) & clnt_addr, & clnt_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return 1;
        }
        response_len = process_buf(buf, cnt, response);

        cnt = sendto(sock, response, response_len, 0,
                (struct sockaddr *) & clnt_addr, clnt_addr_len);
        if (cnt == -1) {
            perror("sendto");
            return 1;
        }
    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}