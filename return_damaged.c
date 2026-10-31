
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RETURN_FILE "returns.txt"
#define ITEM_FILE   "items.txt"
#define LOC_FILE    "locations.txt"
#define STOCK_FILE  "stock.txt"
#define MAX_RECORDS 1000

typedef struct {
    char recordID[16];
    char itemCode[10];
    char locID[10];
    int quantity;
    char type[8];
    char reason[50];
    char date[12];
} ReturnRecord;

typedef struct {
    char itemCode[10];
    char locID[10];
    int quantity;
} StockEntry;

void returnMenu(void);
void processReturn(void);
void reportDamage(void);
void displayReturns(void);
void sortAndDisplayDamageReports(void);
void calculateDamageLossValue(void);

static void flushInputLine(void);
static int  readTextField(const char *prompt, char *buffer, int size);
static int  loadReturns(ReturnRecord rec[], int maxRecords);
static void trimNewline(char s[]);
static void pauseScreen(void);
static void generateRecordID(char *buffer);
static int  validateTextField(const char text[]);
static void getTodayDate(char *buffer, int size);
static int  getItemInfo(const char code[], char nameOut[],
                        int nameOutSize, double *priceOut);
static int  locationExists(const char locID[]);
static int  loadStock(StockEntry stock[], int maxRecords);
static int  saveStock(StockEntry stock[], int n);
static int  findStockIndex(StockEntry stock[], int n,
                           const char itemCode[], const char locID[]);

void returnMenu(void) {
    int choice;
    int running = 1;

    while (running) {
        printf("\n----- Return & Damage Management -----\n");
        printf("1. Process a Returned Item (restock)\n");
        printf("2. Report Damaged / Unusable Item\n");
        printf("3. Display All Return & Damage Records\n");
        printf("4. Damage Reports Sorted by Quantity\n");
        printf("5. Calculate Total Damage Loss (Rs.)\n");
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
            case 1: processReturn();                pauseScreen(); break;
            case 2: reportDamage();                 pauseScreen(); break;
            case 3: displayReturns();               pauseScreen(); break;
            case 4: sortAndDisplayDamageReports();  pauseScreen(); break;
            case 5: calculateDamageLossValue();     pauseScreen(); break;
            case 0: running = 0; break;
            default: printf("Invalid choice.\n");
        }
    }
}

void processReturn(void) {
    char itemCode[10], locID[10], reason[50], recID[16], today[12];
    int qty, idx, n;
    StockEntry stock[MAX_RECORDS];
    FILE *fp;

    printf("\n----- Process a Returned Item (restock) -----\n");

    printf("Enter item code being returned: ");
    if (scanf("%9s", itemCode) != 1) {          
        flushInputLine();
        printf("Invalid input.\n");
        return;
    }
    flushInputLine();
    if (!validateTextField(itemCode)) {          
        printf("Invalid item code.\n");
        return;
    }
    if (!getItemInfo(itemCode, NULL, 0, NULL)) {
        printf("Item \"%s\" does not exist.\n", itemCode);
        return;
    }

    printf("Enter location ID the item is being returned to: ");
    if (scanf("%9s", locID) != 1) {              
        flushInputLine();
        printf("Invalid input.\n");
        return;
    }
    flushInputLine();
    if (!validateTextField(locID)) {             
        printf("Invalid location ID.\n");
        return;
    }
    if (!locationExists(locID)) {
        printf("Location \"%s\" does not exist.\n", locID);
        return;
    }

    printf("Enter quantity returned: ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf("Invalid quantity.\n");
        flushInputLine();
        return;
    }
    flushInputLine();

    if (!readTextField("Reason / condition note: ",   
                       reason, (int)sizeof(reason))) {
        printf("Invalid entry.\n");
        return;
    }

    n = loadStock(stock, MAX_RECORDS);
    idx = findStockIndex(stock, n, itemCode, locID);
    if (idx != -1) {
        stock[idx].quantity += qty;
    } else {
        if (n >= MAX_RECORDS) {
            printf("Stock list is full. Cannot add a new entry.\n");
            return;
        }
        strcpy(stock[n].itemCode, itemCode);
        strcpy(stock[n].locID, locID);
        stock[n].quantity = qty;
        n++;
        idx = n - 1;
    }
    if (!saveStock(stock, n)) {                   
        printf("Error: could not update stock. Return not recorded.\n");
        return;
    }

    generateRecordID(recID);
    getTodayDate(today, (int)sizeof(today));

    fp = fopen(RETURN_FILE, "a");
    if (fp == NULL) {
        printf("Error: could not save return record.\n");
        return;
    }
    fprintf(fp, "%s|%s|%s|%d|RETURN|%s|%s\n",
            recID, itemCode, locID, qty, reason, today);
    fclose(fp);

    printf("Return recorded (%s). Stock of %s at %s is now %d unit(s).\n",
           recID, itemCode, locID, stock[idx].quantity);
}


void reportDamage(void) {
    char itemCode[10], locID[10], reason[50], recID[16], today[12];
    int qty, idx, n;
    StockEntry stock[MAX_RECORDS];
    FILE *fp;

    printf("\n----- Report Damaged / Unusable Item -----\n");

    printf("Enter item code: ");
    if (scanf("%9s", itemCode) != 1) {           
        flushInputLine();
        printf("Invalid input.\n");
        return;
    }
    flushInputLine();
    if (!validateTextField(itemCode)) {          
        printf("Invalid item code.\n");
        return;
    }
    if (!getItemInfo(itemCode, NULL, 0, NULL)) {
        printf("Item \"%s\" does not exist.\n", itemCode);
        return;
    }

    printf("Enter location ID where the damage occurred: ");
    if (scanf("%9s", locID) != 1) {               
        flushInputLine();
        printf("Invalid input.\n");
        return;
    }
    flushInputLine();
    if (!validateTextField(locID)) {              
        printf("Invalid location ID.\n");
        return;
    }
    if (!locationExists(locID)) {
        printf("Location \"%s\" does not exist.\n", locID);
        return;
    }

    n = loadStock(stock, MAX_RECORDS);
    idx = findStockIndex(stock, n, itemCode, locID);

    printf("Enter quantity damaged: ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf("Invalid quantity.\n");
        flushInputLine();
        return;
    }
    flushInputLine();

    if (idx == -1 || stock[idx].quantity < qty) {
        printf("Cannot write off %d unit(s): only %d unit(s) of \"%s\" are in stock at \"%s\".\n",
               qty, (idx == -1) ? 0 : stock[idx].quantity, itemCode, locID);
        return;
    }

    if (!readTextField("Reason (e.g. Screen cracked / Water damage): ",
                       reason, (int)sizeof(reason))) {
        printf("Invalid entry.\n");
        return;
    }

    stock[idx].quantity -= qty;
    if (!saveStock(stock, n)) {                   
        printf("Error: could not update stock. Damage not recorded.\n");
        return;
    }

    generateRecordID(recID);
    getTodayDate(today, (int)sizeof(today));

    fp = fopen(RETURN_FILE, "a");
    if (fp == NULL) {
        printf("Error: could not save damage record.\n");
        return;
    }
    fprintf(fp, "%s|%s|%s|%d|DAMAGE|%s|%s\n",
            recID, itemCode, locID, qty, reason, today);
    fclose(fp);

    printf("Damage recorded (%s). Stock of %s at %s is now %d unit(s).\n",
           recID, itemCode, locID, stock[idx].quantity);
}
void displayReturns(void) {
    ReturnRecord rec[MAX_RECORDS];
    int n = loadReturns(rec, MAX_RECORDS);
    int i;

    printf("\n----- Return & Damage Records (%d) -----\n", n);
    if (n == 0) {
        printf("No return/damage records yet.\n");
        return;
    }

    printf("%-7s %-10s %-10s %5s %-7s %-30s %-10s\n",
           "ID", "ITEM", "LOC", "QTY", "TYPE", "REASON", "DATE");
    printf("--------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
       printf("%-7s %-10s %-10s %5d %-7s %-30s %-10s\n",
               rec[i].recordID, rec[i].itemCode, rec[i].locID,
               rec[i].quantity, rec[i].type, rec[i].reason, rec[i].date);
    }
}

void sortAndDisplayDamageReports(void) {
    ReturnRecord rec[MAX_RECORDS];
    int n = loadReturns(rec, MAX_RECORDS);
    ReturnRecord damaged[MAX_RECORDS];
    ReturnRecord temp;
    int i, j, dCount = 0;

    for (i = 0; i < n; i++) {
        if (strcmp(rec[i].type, "DAMAGE") == 0) {
            damaged[dCount] = rec[i];
            dCount++;
        }
    }

    for (i = 0; i < dCount - 1; i++) {
        for (j = 0; j < dCount - 1 - i; j++) {
            if (damaged[j].quantity < damaged[j + 1].quantity) {
                temp = damaged[j];
                damaged[j] = damaged[j + 1];
                damaged[j + 1] = temp;
            }
        }
    }

    printf("\n----- Damage Reports Sorted by Quantity (highest first) -----\n");
    if (dCount == 0) {
        printf("No damage has been reported yet.\n");
        return;
    }

    printf("%-7s %-10s %-10s %5s %-30s %-10s\n",
           "ID", "ITEM", "LOC", "QTY", "REASON", "DATE");
    printf("--------------------------------------------------------------------\n");
    for (i = 0; i < dCount; i++) {
        printf("%-7s %-8s %-8s %5d %-20s %-10s\n",
               damaged[i].recordID, damaged[i].itemCode, damaged[i].locID,
               damaged[i].quantity, damaged[i].reason, damaged[i].date);
    }
}

void calculateDamageLossValue(void) {
    ReturnRecord rec[MAX_RECORDS];
    int n = loadReturns(rec, MAX_RECORDS);
    int i, anyDamage = 0;
    double totalLoss = 0.0;

    printf("\n----- Total Financial Loss from Damaged Items -----\n");
    printf("%-8s %-24s %5s %12s %12s\n",
           "ITEM", "NAME", "QTY", "UNIT PRICE", "LOSS (Rs.)");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        if (strcmp(rec[i].type, "DAMAGE") == 0) {
            char itemName[60];
            double price = 0.0;
            if (getItemInfo(rec[i].itemCode, itemName,
                            (int)sizeof(itemName), &price)) {
                double loss = price * rec[i].quantity;
                printf("%-8s %-24s %5d %12.2f %12.2f\n",
                       rec[i].itemCode, itemName,
                       rec[i].quantity, price, loss);
                totalLoss += loss;
                anyDamage = 1;
            }
        }
    }

    if (!anyDamage) {
        printf("No damage has been reported yet.\n");
        return;
    }
    printf("--------------------------------------------------------------------\n");
    printf("TOTAL ESTIMATED LOSS: Rs. %.2f\n", totalLoss);
}

static void flushInputLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static int readTextField(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) return 0;

    /* If the newline was NOT read, the user typed more than fits. */
    if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
        flushInputLine();          /* discard the rest of the line */
        printf("Text is too long (maximum %d characters).\n", size - 1);
        return 0;
    }

    trimNewline(buffer);
    return validateTextField(buffer);
}

static int loadReturns(ReturnRecord rec[], int maxRecords) {
    FILE *fp = fopen(RETURN_FILE, "r");
    int count = 0;

    if (fp == NULL) return 0;

    while (count < maxRecords &&
           fscanf(fp, " %15[^|]|%9[^|]|%9[^|]|%d|%7[^|]|%49[^|]|%11[^\r\n]",
                  rec[count].recordID,
                  rec[count].itemCode,
                  rec[count].locID,
                  &rec[count].quantity,
                  rec[count].type,
                  rec[count].reason,
                  rec[count].date) == 7) {
        count++;
    }
    fclose(fp);
    return count;
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

static void generateRecordID(char *buffer) {
    ReturnRecord rec[MAX_RECORDS];
    int n = loadReturns(rec, MAX_RECORDS);
     snprintf(buffer, 16, "RD%04d", n + 1);
}

static int validateTextField(const char text[]) {
    if (strlen(text) == 0) return 0;
    if (strchr(text, '|') != NULL) return 0;
    return 1;
}

static void getTodayDate(char *buffer, int size) {
    time_t now;
    struct tm *t;

    now = time(NULL);
    t = localtime(&now);
    if (t != NULL && strftime(buffer, (size_t)size, "%Y-%m-%d", t) > 0) {
        return;
    }
    strncpy(buffer, "0000-00-00", (size_t)size - 1);
    buffer[size - 1] = '\0';
}
static int getItemInfo(const char code[], char nameOut[],
                       int nameOutSize, double *priceOut) {
    FILE *fp = fopen(ITEM_FILE, "r");
    char lineCode[10], lineName[60], category[25], unit[12];
    double price;
    int found = 0;

    if (fp == NULL) return 0;

    while (fscanf(fp, " %9[^|]|%59[^|]|%24[^|]|%11[^|]|%lf",
                  lineCode, lineName, category, unit, &price) == 5) {
        if (strcmp(lineCode, code) == 0) {
            if (nameOut != NULL && nameOutSize > 0) {
                strncpy(nameOut, lineName, (size_t)nameOutSize - 1);
                nameOut[nameOutSize - 1] = '\0';
            }
            if (priceOut != NULL) *priceOut = price;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;

}

static int locationExists(const char locID[]) {
    FILE *fp = fopen(LOC_FILE, "r");
    char id[10], name[60], deptCode[10], type[15];
    int found = 0;

    if (fp == NULL) return 0;

    while (fscanf(fp, " %9[^|]|%59[^|]|%9[^|]|%14[^\r\n]",
                  id, name, deptCode, type) == 4) {
        if (strcmp(id, locID) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}


static int loadStock(StockEntry stock[], int maxRecords) {
    FILE *fp = fopen(STOCK_FILE, "r");
    int count = 0;

    if (fp == NULL) return 0;

    while (count < maxRecords &&
           fscanf(fp, " %9[^|]|%9[^|]|%d",
                  stock[count].itemCode,
                  stock[count].locID,
                  &stock[count].quantity) == 3) {
        count++;
    }
    fclose(fp);
    return count;
}

static int saveStock(StockEntry stock[], int n) {
    FILE *fp = fopen(STOCK_FILE, "w");
    int i;

    if (fp == NULL) {
        printf("Error: could not update stock file.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        fprintf(fp, "%s|%s|%d\n",
                stock[i].itemCode, stock[i].locID, stock[i].quantity);
    }
    fclose(fp);
    return 1;
}

static int findStockIndex(StockEntry stock[], int n,
                          const char itemCode[], const char locID[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(stock[i].itemCode, itemCode) == 0 &&
            strcmp(stock[i].locID, locID) == 0) {
            return i;
        }
    }
    return -1;
}

