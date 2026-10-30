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

------------------------------------------ menu -----------------------------------------

void itemMenu(void) {
    int choice;
    int running = 1;

    while (running) {
        printf("\n----- Item Registration & Management -----\n");
        printf("1. Register New Item\n");
        printf("2. Display All Items\n");
        printf("3. Search Item\n");
        printf("4. Update Item\n");
        printf("5. Display Items Grouped by Category\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) { running = 0; break; }  /* no more input */
            printf("Invalid input.\n");
            flushInputLine();
            continue;
        }
        flushInputLine();

        switch (choice) {
            case 1: registerItem();           pauseScreen(); break;
            case 2: displayItems();           pauseScreen(); break;
            case 3: searchItem();             pauseScreen(); break;
            case 4: updateItem();             pauseScreen(); break;
            case 5: displayItemsByCategory(); pauseScreen(); break;
            case 0: running = 0; break;
            default: printf("Invalid choice.\n");
        }
    }
}


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

//---------------check valid lists and compare strings without case sensitive-------------------

static int sameIgnoreCase(const char a[], const char b[]) {
    int i;
    char ca, cb;

    for (i = 0; a[i] != '\0' && b[i] != '\0'; i++) {
        ca = a[i];
        cb = b[i];
        if (ca >= 'a' && ca <= 'z') ca = (char)(ca - 32);
        if (cb >= 'a' && cb <= 'z') cb = (char)(cb - 32);
        if (ca != cb) return 0;
    }
    return a[i] == b[i];
}

static int validateCategory(const char category[]) {
    int i;
    for (i = 0; i < NUM_CATEGORIES; i++) {
        if (sameIgnoreCase(category, VALID_CATEGORIES[i])) return 1;
    }
    return 0;
}

----------------------open file and read data in file---------------------------
