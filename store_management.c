#include<stdio.h>
#include<string.h>

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

//main function stock management
void stockMenu(void)
{
    int choice;

    do
    {
        printf("\n===== STOCK MANAGEMENT =====\n");

        printf("1. Add / Increase Stock\n");
        printf("2. Display All Stock\n");
        printf("3. Display Stock by Location\n");
        printf("4. Low Stock Report\n");
        printf("0. Back to Main Menu\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            choice = -1;
            continue;
        }

        switch (choice)
        {
            case 1:
                addStock();
                break;

            case 2:
                displayStock();
                break;

            case 3:
                displayStockByLocation();
                break;

            case 4:
                checkLowStock();
                break;

            case 0:
                printf("Returning to main menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0);
}

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
// Find existing stock
int findStock(StockEntry stock[], int count,
              char code[], char location[])
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (strcmp(stock[i].itemCode, code) == 0 &&
            strcmp(stock[i].locID, location) == 0)
        {
            return i;
        }
    }

    return -1;
}
// option 1:add stock
void addStock(void)
{
    StockEntry stock[MAX];

    char code[10];
    char location[10];

    int quantity;
    int count;
    int index;

    printf("\n--- Add Stock ---\n");

    printf("Enter item code: ");
    scanf("%9s", code);

    if (!itemExists(code))
    {
        printf("Item does not exist!\n");
        return;
    }

    printf("Enter location ID: ");
    scanf("%9s", location);

    if (!locationExists(location))
    {
        printf("Location does not exist!\n");
        return;
    }

    printf("Enter quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0)
    {
        printf("Invalid quantity!\n");
        return;
    }

    count = loadStock(stock);

    index = findStock(stock, count, code, location);

    if (index != -1)
    {
        stock[index].quantity += quantity;
    }
    else
    {
        if (count >= MAX)
        {
            printf("Stock is full!\n");
            return;
        }

        strcpy(stock[count].itemCode, code);
        strcpy(stock[count].locID, location);

        stock[count].quantity = quantity;

        count++;
    }

    saveStock(stock, count);

    printf("Stock successfully updated!\n");
}
//option 2: display all
void displayStock(void)
{
    StockEntry stock[MAX];

    int count;
    int i;

    count = loadStock(stock);

    printf("\n--- All Stock ---\n");

    if (count == 0)
    {
        printf("No stock available!\n");
        return;
    }

    printf("ITEM CODE\tLOCATION\tQUANTITY\n");

    for (i = 0; i < count; i++)
    {
        printf("%s\t\t%s\t\t%d\n",
               stock[i].itemCode,
               stock[i].locID,
               stock[i].quantity);
    }
}
//option 3: display stock location
void displayStockByLocation(void)
{
    StockEntry stock[MAX];

    char location[10];

    int count;
    int i;
    int found = 0;
    int total = 0;

    printf("\nEnter location ID: ");
    scanf("%9s", location);

    if (!locationExists(location))
    {
        printf("Location does not exist!\n");
        return;
    }

    count = loadStock(stock);

    printf("\n--- Stock by Location ---\n");

    for (i = 0; i < count; i++)
    {
        if (strcmp(stock[i].locID, location) == 0)
        {
            printf("Item: %s  Quantity: %d\n",
                   stock[i].itemCode,
                   stock[i].quantity);

            total += stock[i].quantity;

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No stock in this location!\n");
    }
    else
    {
        printf("Total quantity: %d\n", total);
    }
}
//option 4: low stock report
void checkLowStock(void)
{
    StockEntry stock[MAX];

    int count;
    int i;
    int found = 0;

    count = loadStock(stock);

    printf("\n--- Low Stock Report ---\n");

    for (i = 0; i < count; i++)
    {
        if (stock[i].quantity <= LOW_STOCK)
        {
            printf("Item: %s  Location: %s  Quantity: %d\n",
                   stock[i].itemCode,
                   stock[i].locID,
                   stock[i].quantity);

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No low stock items!\n");
    }
}
