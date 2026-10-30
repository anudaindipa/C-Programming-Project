/* ------------------------------------------------------------------
   item_management.c  -  Item Registration & Management

   Data file (one record per line, fields separated by '|'):
     items.txt : code|name|category|unit|unitPrice
   ------------------------------------------------------------------ */

#include <stdio.h>
#include <string.h>

#define ITEM_FILE      "items.txt"
#define MAX_ITEMS      500
#define NUM_CATEGORIES 5

typedef struct {
    char code[10];
    char name[60];
    char category[25];
    char unit[12];
    double price;
} Item;

static const char *VALID_CATEGORIES[NUM_CATEGORIES] = {
    "ELECTRONICS", "FURNITURE", "STATIONERY", "LABEQUIPMENT", "OTHER"
};

//-----------------function of filtering charactors-------------------------

static void flushInputLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

static void trimNewline(char s[]) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        len--;
        s[len] = '\0';
    }
}

static void pauseScreen(void) {
    printf("\nPress ENTER to continue...");
    flushInputLine();
}

static int validateTextField(const char text[]) {
    if (strlen(text) == 0) return 0;
    if (strchr(text, '|') != NULL) return 0;
    return 1;
}