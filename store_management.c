#include <stdio.h>
#include<string.h>

#define STOCK_FILE "stock.txt"
#define ITEM_FILE            "items.txt"
#define LOC_FILE             "locations.txt"
#define MAX_STOCK            1000
#define LOW_STOCK_THRESHOLD  5

typedef struct{
    char itemcode[10];
    char locID[10];
    int quantity;
}Stockentry;

static void flushinputline(void){
    int c;
    while ((c=getchar()) != '\n' &)
}