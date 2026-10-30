#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 1000

typedef struct {
    char id[16], item[10], location[10], person[40], date[12];
    int qty;
} Issue;

typedef struct {
    char item[10], location[10];
    int qty;
} Stock;
static void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void removeNewline(char s[]) {
    s[strcspn(s, "\n")] = '\0';
}



