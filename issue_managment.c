#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 1000

void issueMenu(void) {
    int choice;
    do {
        printf("\n--- Issue Management ---\n");
        printf("1. Issue Item\n2. Display Issued Items\n");
        printf("3. Search by Location\n0. Back\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            clearInput();
            printf("Invalid input.\n");
            continue;
        }
        clearInput();
        switch (choice) {
            case 1: issueItem(); break;
            case 2: displayIssuedItems(); break;
            case 3: searchIssueByLocation(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
        if (choice != 0) {
            printf("\nPress ENTER to continue...");
            getchar();
        }
    } while (choice != 0);
}

