
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