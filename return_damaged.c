
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
