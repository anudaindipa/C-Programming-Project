#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 1000

void issueMenu(void) {
    int choice;
    do {
        printf("\n--- Issue Management ---\n");
        printf("1. Issue Item\n2. Display Issued Items\n");
        printf("3. Search by Location\n0. Back\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            clearInput();
            printf("Invalid input.\n");
            continue;
        }
        clearInput();
        switch (choice) {
            case 1: issueItem(); break;
            case 2: displayIssuedItems(); break;
            case 3: searchIssueByLocation(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
        if (choice != 0) {
            printf("\nPress ENTER to continue...");
            getchar();
        }
    } while (choice != 0);
}

void issueItem(void) {
    char item[10], location[10], name[60], person[40], id[16], date[12];
    Stock stock[MAX];
    int n, i, index = -1, qty;
    FILE *f;
    time_t now;
    struct tm *today;

    printf("\n--- Issue Item ---\n");
    if (!readCode("Item code: ", item)) return;
    if (!findItem(item, name)) {
        printf("Item not found.\n");
        return;
    }
    if (!readCode("Location ID: ", location)) return;
    if (!findLocation(location)) {
        printf("Location not found.\n");
        return;
    }
    printf("Quantity: ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        clearInput();
        printf("Invalid quantity.\n");
        return;
    }
    clearInput();

    n = loadStock(stock);
    if (n < 0) {
        printf("Cannot open stock.txt.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        if (strcmp(stock[i].item, item) == 0 &&
            strcmp(stock[i].location, location) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("No stock record for this item at this location.\n");
        return;
    }
    if (stock[index].qty < qty) {
        printf("Insufficient stock. Available: %d\n", stock[index].qty);
        return;
    }

    printf("Issued to: ");
    if (fgets(person, sizeof person, stdin) == NULL) return;
    if (strchr(person, '\n') == NULL && !feof(stdin)) clearInput();
    removeNewline(person);
    if (person[0] == '\0' || strchr(person, '|') != NULL) {
        printf("Invalid person/purpose.\n");
        return;
    }
}
