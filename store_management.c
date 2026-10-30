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
    while ((c=getchar()) != '\n'  && c != EOF){
        
    }
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
