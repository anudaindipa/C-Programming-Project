#include<stdio.h>
#include<string.h>

#define DEPT_FILE "departments.txt"
#define LOC_FILE "locations.txt"
#define MAX_RECORDS 200 

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



//Load department records from departments.txt into an array.
int loadDepartments(Department departments[]) {
    FILE *fp;
    int count = 0;

    fp = fopen(DEPT_FILE, "r");

    if (fp == NULL) {
        return 0;
    }

    while (count < MAX_RECORDS &&
           fscanf(fp, " %9[^|]|%59[^|]|%11[^\r\n]",
                  departments[count].code,
                  departments[count].name,
                  departments[count].type) == 3) {
        count++;
    }

    fclose(fp);

    return count;
}

void departmentMenu(void) {
    int choice;
    do {
        printf("\n===== Department Management =====\n");
        printf("1. Add Department / Common Area\n");
        printf("2. Display Departments\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) {
                break;
            }
            clearInputBuffer();
            printf("Invalid choice.\n");
            choice = -1;
            continue;
        }

        clearInputBuffer();

        switch (choice) {
            case 1:
                addDepartment();
                break;
            case 2:
                displayDepartments();
                break;
            case 0:
                printf("Returning to main menu.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}