#include <stdio.h>
#include <string.h>

#define DEPT_FILE           "departments.txt"
#define LOC_FILE            "locations.txt"
#define ITEM_FILE           "items.txt"
#define STOCK_FILE          "stock.txt"
#define ISSUE_FILE          "issues.txt"
#define RETURN_FILE         "returns.txt"
#define SUMMARY_FILE        "summary_report.txt"
#define MAX_RECORDS         1000
#define LOW_STOCK_THRESHOLD 5

typedef struct {
    char code[10];
    char name[60];
    char type[12];
} Department;

typedef struct {
    char id[10];
    char name[60];
    char deptCode[10];
    char type[15];
} Location;

typedef struct {
    char code[10];
    char name[60];
    char category[25];
    char unit[12];
    double price;
} Item;

typedef struct {
    char itemCode[10];
    char locID[10];
    int quantity;
} StockEntry;

typedef struct {
    char issueID[16];
    char itemCode[10];
    char locID[10];
    int quantity;
    char issuedTo[40];
    char date[12];
} IssueRecord;

typedef struct {
    char recordID[16];
    char itemCode[10];
    char locID[10];
    int quantity;
    char type[8];
    char reason[50];
    char date[12];
} ReturnRecord;

static void flushInputLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

static void pauseScreen(void) {
    printf("\nPress ENTER to continue...");
    flushInputLine();
}
/* Load data into arrays and return the record count. */
static int loadDepartments(Department d[], int max) {
    FILE *fp = fopen(DEPT_FILE, "r");
    int n = 0;

    if (fp == NULL) return 0;
    while (n < max &&
           fscanf(fp, " %9[^|]|%59[^|]|%11[^\r\n]",
                  d[n].code, d[n].name, d[n].type) == 3) {
        n++;
    }
    fclose(fp);
    return n;
}

static int loadLocations(Location l[], int max) {
    FILE *fp = fopen(LOC_FILE, "r");
    int n = 0;

    if (fp == NULL) return 0;
    while (n < max &&
           fscanf(fp, " %9[^|]|%59[^|]|%9[^|]|%14[^\r\n]",
                  l[n].id, l[n].name, l[n].deptCode, l[n].type) == 4) {
        n++;
    }
    fclose(fp);
    return n;
}

static int loadItems(Item it[], int max) {
    FILE *fp = fopen(ITEM_FILE, "r");
    int n = 0;

    if (fp == NULL) return 0;
    while (n < max &&
           fscanf(fp, " %9[^|]|%59[^|]|%24[^|]|%11[^|]|%lf",
                  it[n].code, it[n].name, it[n].category,
                  it[n].unit, &it[n].price) == 5) {
        n++;
    }
    fclose(fp);
    return n;
}

static int loadStock(StockEntry s[], int max) {
    FILE *fp = fopen(STOCK_FILE, "r");
    int n = 0;

    if (fp == NULL) return 0;
    while (n < max &&
           fscanf(fp, " %9[^|]|%9[^|]|%d",
                  s[n].itemCode, s[n].locID, &s[n].quantity) == 3) {
        n++;
    }
    fclose(fp);
    return n;
}

static int loadIssues(IssueRecord is[], int max) {
    FILE *fp = fopen(ISSUE_FILE, "r");
    int n = 0;

    if (fp == NULL) return 0;
    while (n < max &&
           fscanf(fp, " %15[^|]|%9[^|]|%9[^|]|%d|%39[^|]|%11[^\r\n]",
                  is[n].issueID, is[n].itemCode, is[n].locID,
                  &is[n].quantity, is[n].issuedTo, is[n].date) == 6) {
        n++;
    }
    fclose(fp);
    return n;
}

static int loadReturns(ReturnRecord r[], int max) {
    FILE *fp = fopen(RETURN_FILE, "r");
    int n = 0;

    if (fp == NULL) return 0;
    while (n < max &&
           fscanf(fp, " %15[^|]|%9[^|]|%9[^|]|%d|%7[^|]|%49[^|]|%11[^\r\n]",
                  r[n].recordID, r[n].itemCode, r[n].locID,
                  &r[n].quantity, r[n].type, r[n].reason, r[n].date) == 7) {
        n++;
    }
    fclose(fp);
    return n;
}

/* find item and department */
static int findItemIndex(Item items[], int n, const char code[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(items[i].code, code) == 0) return i;
    }
    return -1;
}


static int findDeptOfLocation(Location locs[], int n,
                              const char locID[], char deptCodeOut[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(locs[i].id, locID) == 0) {
            strcpy(deptCodeOut, locs[i].deptCode);
            return 1;
        }
    }
    return 0;

    /* Counts all records and works out the value of the stock */
void generateInventorySummary(void) {
    Department depts[MAX_RECORDS];
    Location locs[MAX_RECORDS];
    Item items[MAX_RECORDS];
    StockEntry stock[MAX_RECORDS];
    int dCount = loadDepartments(depts, MAX_RECORDS);
    int lCount = loadLocations(locs, MAX_RECORDS);
    int iCount = loadItems(items, MAX_RECORDS);
    int sCount = loadStock(stock, MAX_RECORDS);
    int i, totalUnits = 0;
    double totalValue = 0.0;

    for (i = 0; i < sCount; i++) {
        int idx = findItemIndex(items, iCount, stock[i].itemCode);
        totalUnits += stock[i].quantity;
        if (idx != -1) {
            totalValue += items[idx].price * stock[i].quantity;
        }
    }

    printf("\n================ INVENTORY SUMMARY ================\n");
    printf("Departments / Common Areas registered : %d\n", dCount);
    printf("Locations registered                  : %d\n", lCount);
    printf("Distinct items in catalogue            : %d\n", iCount);
    printf("Total units currently in stock         : %d\n", totalUnits);
    printf("Total value of current stock (Rs.)     : %.2f\n", totalValue);
    printf("=====================================================\n");
}
/* For each department: add up the stock of all its locations. */
void displayDepartmentWiseStock(void) {
    Department depts[MAX_RECORDS];
    Location locs[MAX_RECORDS];
    Item items[MAX_RECORDS];
    StockEntry stock[MAX_RECORDS];
    int dCount = loadDepartments(depts, MAX_RECORDS);
    int lCount = loadLocations(locs, MAX_RECORDS);
    int iCount = loadItems(items, MAX_RECORDS);
    int sCount = loadStock(stock, MAX_RECORDS);
    int d, s;

    printf("\n----- Department-wise Stock Summary -----\n");
    printf("%-10s %-30s %10s %14s\n",
           "DEPT", "NAME", "UNITS", "VALUE (Rs.)");
    printf("--------------------------------------------------------------------\n");

    for (d = 0; d < dCount; d++) {
        int units = 0;
        double value = 0.0;

        for (s = 0; s < sCount; s++) {
            char deptCode[10];
            if (findDeptOfLocation(locs, lCount, stock[s].locID, deptCode) &&
                strcmp(deptCode, depts[d].code) == 0) {
                int idx = findItemIndex(items, iCount, stock[s].itemCode);
                units += stock[s].quantity;
                if (idx != -1) {
                    value += items[idx].price * stock[s].quantity;
                }
            }
        }
        printf("%-10s %-30s %10d %14.2f\n",
               depts[d].code, depts[d].name, units, value);
    }
}

void displayLowStockReport(void) {
    Item items[MAX_RECORDS];
    StockEntry stock[MAX_RECORDS];
    int iCount = loadItems(items, MAX_RECORDS);
    int sCount = loadStock(stock, MAX_RECORDS);
    int i, anyLow = 0;

    printf("\n----- Low Stock Report (threshold = %d) -----\n",
           LOW_STOCK_THRESHOLD);
    printf("%-8s %-24s %-8s %10s\n", "ITEM", "NAME", "LOC", "QTY");
    printf("----------------------------------------------------------\n");
    for (i = 0; i < sCount; i++) {
        if (stock[i].quantity <= LOW_STOCK_THRESHOLD) {
            int idx = findItemIndex(items, iCount, stock[i].itemCode);
            const char *name;
            if (idx != -1) name = items[idx].name;
            else name = "(unknown item)";
            printf("%-8s %-24s %-8s %10d\n",
                   stock[i].itemCode, name, stock[i].locID, stock[i].quantity);
            anyLow = 1;
        }
    }
    if (!anyLow) printf("No low-stock items found.\n");
}

/* Calculates total issued quantity for each item. */
void displayMostIssuedItems(void) {
    Item items[MAX_RECORDS];
    IssueRecord issues[MAX_RECORDS];
    char codes[MAX_RECORDS][10];
    int totals[MAX_RECORDS];
    int uniqueCount = 0;
    int iCount = loadItems(items, MAX_RECORDS);
    int isCount = loadIssues(issues, MAX_RECORDS);
    int i, j;

    for (i = 0; i < isCount; i++) {
        int found = 0;
        for (j = 0; j < uniqueCount; j++) {
            if (strcmp(codes[j], issues[i].itemCode) == 0) {
                totals[j] += issues[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found && uniqueCount < MAX_RECORDS) {
            strcpy(codes[uniqueCount], issues[i].itemCode);
            totals[uniqueCount] = issues[i].quantity;
            uniqueCount++;
        }
    }