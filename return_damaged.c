
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
    scanf("%9s", itemCode);
    flushInputLine();
    if (!getItemInfo(itemCode, NULL, 0, NULL)) {
        printf("Item \"%s\" does not exist.\n", itemCode);
        return;
    }

    printf("Enter location ID the item is being returned to: ");
    scanf("%9s", locID);
    flushInputLine();
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

    printf("Reason / condition note: ");
    fgets(reason, sizeof(reason), stdin);
    trimNewline(reason);
    if (!validateTextField(reason)) {
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
    saveStock(stock, n);

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
    scanf("%9s", itemCode);
    flushInputLine();
    if (!getItemInfo(itemCode, NULL, 0, NULL)) {
        printf("Item \"%s\" does not exist.\n", itemCode);
        return;
    }

    printf("Enter location ID where the damage occurred: ");
    scanf("%9s", locID);
    flushInputLine();
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

    printf("Reason (e.g. Screen cracked / Water damage): ");
    fgets(reason, sizeof(reason), stdin);
    trimNewline(reason);
    if (!validateTextField(reason)) {
        printf("Invalid entry.\n");
        return;
    }

    stock[idx].quantity -= qty;
    saveStock(stock, n);

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

    printf("%-7s %-8s %-8s %5s %-7s %-20s %-10s\n",
           "ID", "ITEM", "LOC", "QTY", "TYPE", "REASON", "DATE");
    printf("--------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-7s %-8s %-8s %5d %-7s %-20s %-10s\n",
               rec[i].recordID, rec[i].itemCode, rec[i].locID,
               rec[i].quantity, rec[i].type, rec[i].reason, rec[i].date);
    }
}

static void flushInputLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
