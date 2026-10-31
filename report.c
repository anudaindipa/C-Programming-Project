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