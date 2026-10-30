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

/* Add a new department or common area. */
 void addDepartment(void) {
    Department department;
    Department departments[MAX_RECORDS];
    FILE *fp;
    int count, i;

    printf("\n--- Add Department / Common Area ---\n");

    printf("Enter code (ICT, ET, BST, MDC, COMMON): ");
    scanf("%9s", department.code);
    clearInputBuffer();
    makeUppercase(department.code);

    count = loadDepartments(departments);

    for (i = 0; i < count; i++) {
        if (strcmp(departments[i].code, department.code) == 0) {
            printf("This department code already exists.\n");
            return;
        }
    }

    printf("Enter department / area name: ");
    fgets(department.name, sizeof(department.name), stdin);
    removeNewline(department.name);

    printf("Enter type (ACADEMIC / COMMON): ");
    scanf("%11s", department.type);
    clearInputBuffer();
    makeUppercase(department.type);

    if (strcmp(department.name, "") == 0 ||
        strchr(department.name, '|') != NULL ||
        (strcmp(department.type, "ACADEMIC") != 0 &&
         strcmp(department.type, "COMMON") != 0)) {
        printf("Invalid details. Department was not added.\n");
        return;
    }

    fp = fopen(DEPT_FILE, "a");

    if (fp == NULL) {
        printf("Cannot open departments.txt\n");
        return;
    }

    fprintf(fp, "%s|%s|%s\n",
            department.code, department.name, department.type);
    fclose(fp);

    printf("Department added successfully.\n");
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