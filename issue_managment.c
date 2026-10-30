/* ------------------------------------------------------------------
   issue_management.c  -  Issue / Allocation Management

<<<<<<< HEAD
   Data files (one record per line, fields separated by '|'):*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define ISSUE_FILE  "issues.txt"
#define ITEM_FILE   "items.txt"
#define LOC_FILE    "locations.txt"
#define STOCK_FILE  "stock.txt"
#define MAX_RECORDS 1000