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

/* Display all departments or common areas. */
void displayDepartments(void) {
    Department departments[MAX_RECORDS];
    int count, i;

    count = loadDepartments(departments);

    printf("\n--- Department List ---\n");

    if (count == 0) {
        printf("No departments found.\n");
        return;
    }

    printf("%-10s %-30s %-10s\n", "CODE", "NAME", "TYPE");

    for (i = 0; i < count; i++) {
        printf("%-10s %-30s %-10s\n",
               departments[i].code,
               departments[i].name,
               departments[i].type);
    }
}

void searchDepartment(void) {
    Department departments[MAX_RECORDS];
    char keyword[60];
    int count, i, found = 0;

    count = loadDepartments(departments);

    printf("Enter code or name to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    removeNewline(keyword);

    for (i = 0; i < count; i++) {
        if (strstr(departments[i].code, keyword) != NULL ||
            strstr(departments[i].name, keyword) != NULL) {
            printf("%s | %s | %s\n",
                   departments[i].code,
                   departments[i].name,
                   departments[i].type);
            found = 1;
        }
    }

    if (found == 0) {
        printf("No matching department found.\n");
    }
}


/* Update the name and type of an existing department. */
static void updateDepartment(void) {
    Department departments[MAX_RECORDS];
    char code[10];
    char newName[60];
    char newType[12];
    FILE *fp;
    int count, i, found = -1;

    count = loadDepartments(departments);

    printf("Enter department code to update: ");
    scanf("%9s", code);
    clearInputBuffer();
    makeUppercase(code);

    for (i = 0; i < count; i++) {
        if (strcmp(departments[i].code, code) == 0) {
            found = i;
        }
    }

    if (found == -1) {
        printf("Department not found.\n");
        return;
    }

    printf("Current name: %s\n", departments[found].name);
    printf("Enter new name (ENTER to keep): ");
    fgets(newName, sizeof(newName), stdin);
    removeNewline(newName);

    if (strcmp(newName, "") != 0) {
        if (strchr(newName, '|') != NULL) {
            printf("Name cannot contain the | character. Update cancelled.\n");
            return;
        }
        strcpy(departments[found].name, newName);
    }

    printf("Current type: %s\n", departments[found].type);
    printf("Enter new type (ACADEMIC / COMMON): ");
    fgets(newType, sizeof(newType), stdin);
    removeNewline(newType);
    makeUppercase(newType);

    if (strcmp(newType, "") != 0) {
        if (strcmp(newType, "ACADEMIC") != 0 &&
            strcmp(newType, "COMMON") != 0) {
            printf("Invalid type. Update cancelled.\n");
            return;
        }

        strcpy(departments[found].type, newType);
    }

    fp = fopen(DEPT_FILE, "w");

    if (fp == NULL) {
        printf("Cannot save department data.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%s|%s|%s\n",
                departments[i].code,
                departments[i].name,
                departments[i].type);
    }

    fclose(fp);
    printf("Department updated successfully.\n");
}


/* Read location records from locations.txt into an array. */
static int loadLocations(Location locations[]) {
    FILE *fp;
    int count = 0;

    fp = fopen(LOC_FILE, "r");

    if (fp == NULL) {
        return 0;
    }

    while (count < MAX_RECORDS &&
           fscanf(fp, " %9[^|]|%59[^|]|%9[^|]|%14[^\r\n]",
                  locations[count].id,
                  locations[count].name,
                  locations[count].deptCode,
                  locations[count].type) == 4) {
        count++;
    }

    fclose(fp);

    return count;
}


/* Check whether a department code is already saved. */
static int isDepartmentExist(char code[]) {
    Department departments[MAX_RECORDS];
    int count, i;

    count = loadDepartments(departments);

    for (i = 0; i < count; i++) {
        if (strcmp(departments[i].code, code) == 0) {
            return 1;
        }
    }

    return 0;
}

/* Check whether a location ID is already saved. */
static int isLocationExist(char id[]) {
    Location locations[MAX_RECORDS];
    int count, i;

    count = loadLocations(locations);

    for (i = 0; i < count; i++) {
        if (strcmp(locations[i].id, id) == 0) {
            return 1;
        }
    }

    return 0;
}

void departmentMenu(void) {
    int choice;
    do {
        printf("\n===== Department Management =====\n");
        printf("1. Add Department / Common Area\n");
        printf("2. Display Departments\n");
        printf("3. Search Department\n");
        printf("4. Update Department\n");
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
            case 3:
                searchDepartment();
                break;
            case 4:
                updateDepartment();
                break;
            case 0:
                printf("Returning to main menu.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}