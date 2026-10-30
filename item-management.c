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


void registerItem(void) {
    Item it;
    FILE *fp;
    char *code;

    printf("\n----- Register New Item -----\n");

    code = generateItemCode();
    strcpy(it.code, code);
    printf("Auto-generated item code: %s\n", it.code);

    printf("Enter item name: ");
    fgets(it.name, sizeof(it.name), stdin);
    trimNewline(it.name);
    if (!validateTextField(it.name)) {
        printf("Invalid item name.\n");
        return;
    }

    printf("Enter category (Electronics / Furniture / Stationery / LabEquipment / Other): ");
    fgets(it.category, sizeof(it.category), stdin);
    trimNewline(it.category);
    if (!validateCategory(it.category)) {
        printf("Invalid category.\n");
        return;
    }

    printf("Enter unit of measure (Nos / Box / Pack / Set): ");
    scanf("%11s", it.unit);
    flushInputLine();
    if (!validateTextField(it.unit)) {
        printf("Invalid unit.\n");
        return;
    }

    printf("Enter unit price (Rs.): ");
    if (scanf("%lf", &it.price) != 1 || it.price < 0) {
        printf("Invalid price.\n");
        flushInputLine();
        return;
    }
    flushInputLine();

    fp = fopen(ITEM_FILE, "a");
    if (fp == NULL) {
        printf("Error: could not open %s for writing.\n", ITEM_FILE);
        return;
    }
    fprintf(fp, "%s|%s|%s|%s|%.2f\n",
            it.code, it.name, it.category, it.unit, it.price);
    fclose(fp);

    printf("Item \"%s\" (%s) registered successfully.\n", it.code, it.name);
}


// ------------- menu fuction ----------------

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
            if (feof(stdin)) { running = 0; break; }
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