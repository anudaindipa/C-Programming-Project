#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 1000
/* Display main function */
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
/* option 1 issueItem function*/
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
     nextIssueID(id);
    now = time(NULL);
    today = localtime(&now);
    if (today != NULL) strftime(date, sizeof date, "%Y-%m-%d", today);
    else strcpy(date, "0000-00-00");

    /* Open the issue file before changing stock. */
    f = fopen("issues.txt", "a");
    if (f == NULL) {
        printf("Cannot open issues.txt. Stock unchanged.\n");
        return;
    }
    stock[index].qty -= qty;
    if (!saveStock(stock, n)) {
        fclose(f);
        printf("Could not save stock. Issue was not recorded. Check files.\n");
        return;
    }
    i = fprintf(f, "%s|%s|%s|%d|%s|%s\n",
          id, item, location, qty, person, date);
    if (fclose(f) != 0 || i < 0) {
        printf("Issue record save failed. Check stock and issue files.\n");
        return;
    }
    printf("Issued %d x %s (%s) at %s. ID: %s, Date: %s\n",
           qty, name, item, location, id, date);
    printf("Remaining stock: %d\n", stock[index].qty);
}
