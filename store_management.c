#include <stdio.h>
#include <string.h>

#define MAX 1000
#define STOCK_FILE "stock.txt"
#define ITEM_FILE "items.txt"
#define LOC_FILE "locations.txt"
#define LOW_STOCK 5

// Structure for stock information
typedef struct
{
    char itemCode[10];
    char locID[10];
    int quantity;

} StockEntry;

// Function declarations
void addStock(void);
void displayStock(void);
void displayStockByLocation(void);
void checkLowStock(void);
void stockMenu(void);

// Check whether item exists
int itemExists(char code[])
{
    FILE *fp;
    char line[200];
    char fileCode[10];

    fp = fopen(ITEM_FILE, "r");

    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        sscanf(line, "%9[^|]", fileCode);

        if (strcmp(code, fileCode) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}


int locationExists(char id[])
{
    FILE *fp;
    char line[200];
    char fileID[10];

    fp = fopen(LOC_FILE, "r");

    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        sscanf(line, "%9[^|]", fileID);

        if (strcmp(id, fileID) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}

// Load stock information
int loadStock(StockEntry stock[])
{
    FILE *fp;
    int count = 0;

    fp = fopen(STOCK_FILE, "r");

    if (fp == NULL)
        return 0;

    while (count < MAX &&
           fscanf(fp, " %9[^|]|%9[^|]|%d",
                  stock[count].itemCode,
                  stock[count].locID,
                  &stock[count].quantity) == 3)
    {
        count++;
    }

    fclose(fp);

    return count;
}


// Save stock information
void saveStock(StockEntry stock[], int count)
{
    FILE *fp;
    int i;

    fp = fopen(STOCK_FILE, "w");

    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%s|%s|%d\n",
                stock[i].itemCode,
                stock[i].locID,
                stock[i].quantity);
    }

    fclose(fp);
}

