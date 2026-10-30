
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1000

typedef struct {
    char id[16], item[10], location[10], type[8], reason[50], date[12];
    int quantity;
} Record;

typedef struct {
    char item[10], location[10];
    int quantity;
} Stock;

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

int readNumber(void) {
    int number;
    if (scanf("%d", &number) != 1) {
        clearInput();
        return -1;
    }
    clearInput();
    return number;
}

