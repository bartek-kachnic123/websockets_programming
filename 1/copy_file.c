#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Użycie: %s plik_wejsciowy plik_wyjsciowy\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd_read = open(argv[1], O_RDONLY);

    if (fd_read == -1)
    {
        perror("Blad otwarcia pliku wejsciowego");
        exit(EXIT_FAILURE);
    
    }
    int fd_write = open(argv[2], O_WRONLY);

    if (fd_write == -1) {
        perror("Blad otwarcia pliku wyjsciowego");
        exit(EXIT_FAILURE);
    }

    const int BUF_SIZE = 16;
    char buf[BUF_SIZE];
    size_t bytes_read;

    while ((bytes_read = read(fd_read, buf, BUF_SIZE)) > 0) {
        
        if (write(fd_write, buf, bytes_read) != bytes_read) {
            perror("Błąd zapisu do pliku wyjściowego");

            return EXIT_FAILURE;
        }
    }


    if (close(fd_read) == -1) {
        perror("Blad zamknięcia pliku wejsciowego");
        exit(EXIT_FAILURE);
    }
    if (close(fd_write) == -1) {
        perror("Blad zamknięcia pliku wyjsciowego");
        exit(EXIT_FAILURE);
    }
   

    printf("Plik skopiowany pomyślnie.\n");
    return EXIT_SUCCESS;
}
