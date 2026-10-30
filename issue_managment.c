#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 1000

typedef struct {
    char id[16];
    char item[10];
    char location[10];
    char person[40];
    char date[12];
    int qty;
} Issue;

typedef struct {
    char item[10];
    char location[10];
    int qty;
} Stock;

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
// option 2 displayIssueditems function
void displayIssuedItems(void) {
    Issue issues[MAX];
    int n = loadIssues(issues), i;
    printf("\n--- All Issued Items (%d) ---\n", n);
    if (n == 0) {
        printf("No issue records found.\n");
        return;
    }
    printf("%-10s %-9s %-9s %5s %-25s %s\n",
           "ID", "ITEM", "LOCATION", "QTY", "ISSUED TO", "DATE");
    for (i = 0; i < n; i++) {
        printf("%-10s %-9s %-9s %5d %-25s %s\n",
               issues[i].id, issues[i].item, issues[i].location,
               issues[i].qty, issues[i].person, issues[i].date);
    }
}
// Adding option 3 searchIssueByLocation function
void searchIssueByLocation(void) {
    Issue issues[MAX];
    char location[10];
    int n = loadIssues(issues), i, found = 0, total = 0;
    if (!readCode("Location ID: ", location)) return;
    printf("\n--- Issues at %s ---\n", location);
    for (i = 0; i < n; i++) {
        if (strcmp(issues[i].location, location) == 0) {
            printf("%s | %s | Qty: %d | %s | %s\n",
                   issues[i].id, issues[i].item, issues[i].qty,
                   issues[i].person, issues[i].date);
            total += issues[i].qty;
            found = 1;
        }
    }
    if (found) printf("Total units issued: %d\n", total);
    else printf("No issues found for this location.\n");
}
typedef struct {
    char id[16], item[10], location[10], person[40], date[12];
    int qty;
} Issue;

typedef struct {
    char item[10], location[10];
    int qty;
} Stock; 
// Remove any extra characters 
static void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void removeNewline(char s[]) {
    s[strcspn(s, "\n")] = '\0';
}
//read a single word
static int readCode(const char prompt[], char code[]) {
    printf("%s", prompt);
    if (scanf("%9s", code) != 1) {
        clearInput();
        return 0;
    }
    clearInput();
    return 1;
}
// findItem function
static int findItem(const char code[], char itemName[]) {
    FILE *f = fopen("items.txt", "r");
    char line[220], id[10], name[60], category[25], unit[12];
    double price;
    if (f == NULL) return 0;
    while (fgets(line, sizeof line, f)) {
        if (sscanf(line, "%9[^|]|%59[^|]|%24[^|]|%11[^|]|%lf",
                   id, name, category, unit, &price) == 5 && strcmp(id, code) == 0) {
            strcpy(itemName, name);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
// findLocation function
static int findLocation(const char code[]) {
    FILE *f = fopen("locations.txt", "r");
    char line[220], id[10], name[60], department[10], type[15];
    if (f == NULL) return 0;
    while (fgets(line, sizeof line, f)) {
        if (sscanf(line, "%9[^|]|%59[^|]|%9[^|]|%14[^\n]",
                   id, name, department, type) == 4 && strcmp(id, code) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
//loadstock function
static int loadStock(Stock stock[]) {
    FILE *f = fopen("stock.txt", "r");
    char line[100];
    int n = 0;
    if (f == NULL) return -1;
    while (n < MAX && fgets(line, sizeof line, f)) {
        if (sscanf(line, "%9[^|]|%9[^|]|%d", stock[n].item,
                   stock[n].location, &stock[n].qty) == 3) n++;
    }
    fclose(f);
    return n;
}
// savestock
static int saveStock(Stock stock[], int n) {
    FILE *f = fopen("stock.txt", "w");
    int i, ok = 1;
    if (f == NULL) return 0;
    for (i = 0; i < n; i++) {
        if (fprintf(f, "%s|%s|%d\n", stock[i].item,
                    stock[i].location, stock[i].qty) < 0) ok = 0;
    }
    if (fclose(f) != 0) ok = 0;
    return ok;
}

// loadIssues 
static int loadIssues(Issue issues[]) {
    FILE *f = fopen("issues.txt", "r");
    char line[200];
    int n = 0;
    if (f == NULL) return 0;
    while (n < MAX && fgets(line, sizeof line, f)) {
        if (sscanf(line, "%15[^|]|%9[^|]|%9[^|]|%d|%39[^|]|%11[^\n]",
                   issues[n].id, issues[n].item, issues[n].location,
                   &issues[n].qty, issues[n].person, issues[n].date) == 6) n++;
    }
    fclose(f);
    return n;
}
//nextIssueID
static void nextIssueID(char id[]) {
    Issue issues[MAX];
    int n = loadIssues(issues), i, number, largest = 0;
    for (i = 0; i < n; i++) {
        if (sscanf(issues[i].id, "ISS%d", &number) == 1 && number > largest)
            largest = number;
    }
    sprintf(id, "ISS%04d", largest + 1);
}
