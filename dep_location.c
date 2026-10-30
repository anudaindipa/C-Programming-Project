#include<stdio.h>
#include<string.h>

#define DEPT_FILE "departments.txt"
#define LOC_FILE "locations.txt"
#define MAX_RECORDS 200 


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
}/* tharusha*/
