
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1000

typedef struct {
    char id[16], item[10], location[10], type[8], reason[50], date[12];
    int quantity;
} Record;

typedef struct {
    char item[10], location[10];
    int quantity;
} Stock;

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

int readNumber(void) {
    int number;
    if (scanf("%d", &number) != 1) {
        clearInput();
        return -1;
    }
    clearInput();
    return number;
}

int loadRecords(Record records[]) {
    FILE *file = fopen("returns.txt", "r");
    int count = 0;
    if (file == NULL) return 0;
    while (count < MAX &&
           fscanf(file, " %15[^|]|%9[^|]|%9[^|]|%d|%7[^|]|%49[^|]|%11[^\r\n]",
                  records[count].id, records[count].item,
                  records[count].location, &records[count].quantity,
                  records[count].type, records[count].reason,
                  records[count].date) == 7) {
        count++;
    }
    fclose(file);
    return count;
}

void displayRecords(void) {
    Record records[MAX];
    int count = loadRecords(records);
    int i;
    printf("\n--- All Return and Damage Records ---\n");
    if (count == 0) {
        printf("No records found.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        printf("%s | %s | %s | %d | %s | %s | %s\n",
               records[i].id, records[i].item, records[i].location,
               records[i].quantity, records[i].type,
               records[i].reason, records[i].date);
    }
}

int itemExists(char code[]) {
    FILE *file = fopen("items.txt", "r");
    char item[10], name[60], category[25], unit[12];
    double price;
    if (file == NULL) return 0;
    while (fscanf(file, " %9[^|]|%59[^|]|%24[^|]|%11[^|]|%lf",
                  item, name, category, unit, &price) == 5) {
        if (strcmp(item, code) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int locationExists(char code[]) {
    FILE *file = fopen("locations.txt", "r");
    char id[10], name[60], dept[10], type[15];
    if (file == NULL) return 0;
    while (fscanf(file, " %9[^|]|%59[^|]|%9[^|]|%14[^\r\n]",
                  id, name, dept, type) == 4) {
        if (strcmp(id, code) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void returnMenu(void) {
    int choice;
    do {
        printf("\n===== Return & Damage Menu =====\n");
        printf("1. Process a returned item (coming soon)\n");
        printf("2. Report damaged item (coming soon)\n");
        printf("3. Display all return/damage records\n");
        printf("4. Sort damage reports by quantity (coming soon)\n");
        printf("5. Calculate total financial loss (coming soon)\n");
        printf("0. Back to main menu\nChoose: ");
        choice = readNumber();
        switch (choice) {
            case 1: printf("Option will be added in a later commit.\n"); break;
            case 2: printf("Option will be added in a later commit.\n"); break;
            case 3: displayRecords(); break;
            case 4: printf("Option will be added in a later commit.\n"); break;
            case 5: printf("Option will be added in a later commit.\n"); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}
