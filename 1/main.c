#include <stdio.h>

void print_numbers(int numbers[], int length) {
    for (int i = 0; i < length; i++) {
        if (numbers[i] > 10 && numbers[i] < 100)
            printf("%d ", numbers[i]);
    }
}

void print_numbers_alt(int* numbers, int length) {
    int *end = numbers + length;

    for (int *p = numbers; p != end; ++p) {
        if (*p > 10 && *p < 100)
            printf("%d ", *p);
    }
}


int main() {
    int numbers[50];
    int num, i;
    
    for (i = 0; i < 50; ++i) {
        scanf("%d", &num);

        if (num == 0)
            break;

        numbers[i] = num;
    }

    print_numbers(numbers, i);
    printf("\n");
    print_numbers_alt(numbers, i);

}