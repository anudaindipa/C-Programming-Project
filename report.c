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