#include <stdio.h>
#include <string.h>

// ------------- menu fuction ----------------

void itemMenu(void) {
    int choice;
    int running = 1;

    while (running) {
        printf("\n----- Item Registration & Management -----\n");
        printf("1. Register New Item\n");
        printf("2. Display All Items\n");
        printf("3. Search Item\n");
        printf("4. Update Item\n");
        printf("5. Display Items Grouped by Category\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) { running = 0; break; }  /* no more input */
            printf("Invalid input.\n");
            flushInputLine();
            continue;
        }
        flushInputLine();

        switch (choice) {
            case 1: registerItem();           pauseScreen(); break;
            case 2: displayItems();           pauseScreen(); break;
            case 3: searchItem();             pauseScreen(); break;
            case 4: updateItem();             pauseScreen(); break;
            case 5: displayItemsByCategory(); pauseScreen(); break;
            case 0: running = 0; break;
            default: printf("Invalid choice.\n");
        }
    }
}