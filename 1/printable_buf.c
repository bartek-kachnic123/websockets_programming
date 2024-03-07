#include <stdio.h>
#include <stdbool.h>

bool printable_buf(const void *buf, int len) {
    const unsigned char *p = (const unsigned char *) buf;
    const unsigned char *end = p + len;
    for (; p != end ; ++p) {
        if (*p < 32 || *p > 126)
            return false;  
    }

    return true; 
}

bool printable_buf_alt(const char *buf) {
    const unsigned char *p = (const unsigned char *)buf; 

    while (*p != '\0') { 
        if (*p < 32 || *p > 126) { 
            return false; 
        }
        p++; 
    }

    return true; 
}

void test_printable_buf() {
    char buf1[] = {'H', 'e', 'l', 'l','o'}; // Bufor zawierający tylko drukowalne znaki ASCII
    char buf2[] = {'H', '\x01', 'e', 'l','l', 'o'}; // Bufor zawierający niedrukowalny znak ASCII (znak o wartości 1)
    if (printable_buf(buf1, 5)) {
        printf("Bufor 1 zawiera tylko drukowalne znaki ASCII.\n");
    } else {
        printf("Bufor 1 zawiera co najmniej jeden niedrukowalny znak ASCII.\n");
    }

    if (printable_buf(buf2, 6)) {
        printf("Bufor 2 zawiera tylko drukowalne znaki ASCII.\n");
    } else {
        printf("Bufor 2 zawiera co najmniej jeden niedrukowalny znak ASCII.\n");
    }
}

void test_printable_buf_alt() {
     char buf1[] = {'H', 'e', 'l', 'l', 'o', '\0'}; // Bufor zawierający tylko drukowalne znaki ASCII
    char buf2[] = "Hello, \x01World!"; // Bufor zawierający niedrukowalny znak ASCII (znak o wartości 1)
    
    if (printable_buf_alt(buf1)) {
        printf("Bufor 1 zawiera tylko drukowalne znaki ASCII.\n");
    } else {
        printf("Bufor 1 zawiera co najmniej jeden niedrukowalny znak ASCII.\n");
    }

    if (printable_buf_alt(buf2)) {
        printf("Bufor 2 zawiera tylko drukowalne znaki ASCII.\n");
    } else {
        printf("Bufor 2 zawiera co najmniej jeden niedrukowalny znak ASCII.\n");
    }
}

int main() {
    test_printable_buf();
    test_printable_buf_alt();
}