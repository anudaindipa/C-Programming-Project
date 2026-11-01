# Faculty of Technology - Inventory Management System

A simple command-line program written in C to manage a faculty's inventory. It uses text files to store data.

## Features

- **Departments & Locations:** Add, view, search, update and delete departments; add and view locations.
- **Items:** Register, view, search and update items; group items by category.
- **Stock:** Add stock, view stock by location and check low stock.
- **Issues:** Issue items, view issued items and search issues by location.
- **Returns & Damages:** Record returns and damages, update stock and calculate damage loss.
- **Reports:** View inventory summaries, low stock, most issued items, damage loss and export a summary report.

## Requirements

- A C compiler (GCC recommended)
- A terminal or command prompt

## How to Compile and Run

Open a terminal in the project folder and compile all source files:

```sh
gcc Main.c dep_location.c item-management.c store_management.c issue_managment.c return_damaged.c report.c -o inventory
```

Run the program:

**Windows:**
```sh
inventory.exe
```

**Linux / macOS:**
```sh
./inventory
```

Enter a menu number to use a feature. Enter `0` to go back or exit.

## Source Files

| File | Purpose |
| --- | --- |
| `Main.c` | Main menu and program entry point |
| `dep_location.c` | Department and location management |
| `item-management.c` | Item management |
| `store_management.c` | Stock management |
| `issue_managment.c` | Item issuing |
| `return_damaged.c` | Returns and damage management |
| `report.c` | Reports and statistics |

## Data Files

The program reads and updates `departments.txt`, `locations.txt`, `items.txt`, `stock.txt`, `issues.txt` and `returns.txt`. Sample data is included.

Select **Reports & Statistics → Export Summary Report to File** to create or update `summary_report.txt`. A sample report is included.

**Note:** Keep the source files and data files in the same project folder. Compile and run the program from that folder so it can find the data files.
