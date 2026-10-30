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

//------------- item operations -------------

//register item...
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

//display item...
void displayItems(void) {
    Item items[MAX_ITEMS];
    int n = loadItems(items, MAX_ITEMS);
    int i;

    printf("\n----- Registered Items (%d) -----\n", n);
    if (n == 0) {
        printf("No items registered yet.\n");
        return;
    }

    printf("%-8s %-24s %-14s %-6s %10s\n",
           "CODE", "NAME", "CATEGORY", "UNIT", "PRICE");
    printf("----------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-8s %-24s %-14s %-6s %10.2f\n",
               items[i].code, items[i].name, items[i].category,
               items[i].unit, items[i].price);
    }
}

//scarch items...
void searchItem(void) {
    Item items[MAX_ITEMS];
    int n = loadItems(items, MAX_ITEMS);
    char keyword[60];
    int i, found = 0;

    printf("\nEnter item code or part of the item name to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    trimNewline(keyword);

    for (i = 0; i < n; i++) {
        if (strstr(items[i].code, keyword) != NULL ||
            strstr(items[i].name, keyword) != NULL) {
            if (!found) {
                printf("%-8s %-24s %-14s %-6s %10s\n",
                       "CODE", "NAME", "CATEGORY", "UNIT", "PRICE");
                printf("----------------------------------------------------------------------\n");
            }
            printf("%-8s %-24s %-14s %-6s %10.2f\n",
                   items[i].code, items[i].name, items[i].category,
                   items[i].unit, items[i].price);
            found = 1;
        }
    }
    if (!found) printf("No matching item found.\n");
}

//item update...
void updateItem(void) {
    Item items[MAX_ITEMS];
    int n = loadItems(items, MAX_ITEMS);
    char code[10];
    int i, idx = -1;
    FILE *fp;

    printf("\nEnter item code to update: ");
    scanf("%9s", code);
    flushInputLine();

    for (i = 0; i < n; i++) {
        if (strcmp(items[i].code, code) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Item \"%s\" not found.\n", code);
        return;
    }

    printf("Current name: %s\nEnter new name: ", items[idx].name);
    fgets(items[idx].name, sizeof(items[idx].name), stdin);
    trimNewline(items[idx].name);
    if (!validateTextField(items[idx].name)) {
        printf("Invalid name. Update cancelled.\n");
        return;
    }

    printf("Current price: %.2f\nEnter new price: ", items[idx].price);
    if (scanf("%lf", &items[idx].price) != 1 || items[idx].price < 0) {
        printf("Invalid price. Update cancelled.\n");
        flushInputLine();
        return;
    }
    flushInputLine();

    fp = fopen(ITEM_FILE, "w");
    if (fp == NULL) {
        printf("Error: could not save changes.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        fprintf(fp, "%s|%s|%s|%s|%.2f\n",
                items[i].code, items[i].name, items[i].category,
                items[i].unit, items[i].price);
    }
    fclose(fp);
    printf("Item \"%s\" updated successfully.\n", code);
}

//display item by category...
void displayItemsByCategory(void) {
    Item items[MAX_ITEMS];
    int n = loadItems(items, MAX_ITEMS);
    int counted[MAX_ITEMS];
    int i, j, count;
    double totalValue;

    for (i = 0; i < n; i++) counted[i] = 0;

    printf("\n----- Items Grouped by Category -----\n");
    if (n == 0) {
        printf("No items registered yet.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        if (counted[i]) continue;   /* already printed in an earlier group */

        count = 0;
        totalValue = 0.0;
        printf("\nCategory: %s\n", items[i].category);
        printf("%-8s %-24s %10s\n", "CODE", "NAME", "PRICE");

        for (j = i; j < n; j++) {
            if (!counted[j] &&
                strcmp(items[j].category, items[i].category) == 0) {
                printf("%-8s %-24s %10.2f\n",
                       items[j].code, items[j].name, items[j].price);
                counted[j] = 1;
                count++;
                totalValue += items[j].price;
            }
        }
        printf("  -> %d item(s), combined listed value Rs. %.2f\n",
               count, totalValue);
    }
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