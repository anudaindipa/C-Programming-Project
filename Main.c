
#include <stdio.h>

extern void departmentMenu(void);   
extern void itemMenu(void);        
extern void stockMenu(void);        
extern void issueMenu(void);       
extern void returnMenu(void);      
extern void reportMenu(void);       

static void printMainMenu(void);
static void flushInputLine(void);

int main(void) {
    int choice;
    int running = 1;

    printf("=============================================================\n");
    printf("   FACULTY OF TECHNOLOGY - INVENTORY MANAGEMENT SYSTEM\n");
    printf("=============================================================\n");

    while (running) {
        printMainMenu();

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) { running = 0; break; }  
            printf("\nInvalid input. Please enter a number.\n");
            flushInputLine();
            continue;
        }
        flushInputLine();

        switch (choice) {
            case 1: departmentMenu(); break;
            case 2: itemMenu();       break;
            case 3: stockMenu();      break;
            case 4: issueMenu();      break;
            case 5: returnMenu();     break;
            case 6: reportMenu();     break;
            case 0:
                printf("\nShutting down the Inventory Management System. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("\nInvalid choice. Please select an option between 0 and 6.\n");
        }
    }

    return 0;
}

static void printMainMenu(void) {
    printf("\n--------------------------- MAIN MENU ----------------------------\n");
    printf(" 1. Department & Location Management\n");
    printf(" 2. Item Registration & Management\n");
    printf(" 3. Stock Management\n");
    printf(" 4. Issue / Allocation Management\n");
    printf(" 5. Return & Damage Management\n");
    printf(" 6. Reports & Statistics\n");
    printf(" 0. Exit System\n");
    printf("--------------------------------------------------------------------\n");
    printf("Enter your choice: ");
}


static void flushInputLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
