/* ------------------------------------------------------------------
   item_management.c  -  Item Registration & Management

   Data file (one record per line, fields separated by '|'):
     items.txt : code|name|category|unit|unitPrice
   ------------------------------------------------------------------ */

#include <stdio.h>
#include <string.h>

#define ITEM_FILE      "items.txt"
#define MAX_ITEMS      500
#define NUM_CATEGORIES 5

typedef struct {
    char code[10];
    char name[60];
    char category[25];
    char unit[12];
    double price;
} Item;

static const char *VALID_CATEGORIES[NUM_CATEGORIES] = {
    "ELECTRONICS", "FURNITURE", "STATIONERY", "LABEQUIPMENT", "OTHER"
};
