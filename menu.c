/*
 * Restaurant Console Application
 * ---------------------------------
 * A simple interactive console app that lets a customer:
 *   - Browse a categorized menu
 *   - Add items to an order (with quantity)
 *   - View / remove items from the current order
 *   - See a running subtotal, tax, and total
 *   - Checkout and print a receipt
 *
 * Compile:  gcc restaurant_menu.c -o restaurant_menu
 * Run:      ./restaurant_menu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MENU_ITEMS   30
#define MAX_ORDER_ITEMS  30
#define NAME_LEN         50
#define TAX_RATE         0.08   /* 8% sales tax */

typedef struct {
    int   id;
    char  name[NAME_LEN];
    float price;
    char  category[20];
} MenuItem;

typedef struct {
    int   menuId;
    int   quantity;
} OrderItem;

/* ---------- Global Menu Data ---------- */
MenuItem menu[MAX_MENU_ITEMS];
int menuCount = 0;

/* ---------- Global Order Data ---------- */
OrderItem order[MAX_ORDER_ITEMS];
int orderCount = 0;

/* ---------- Function Prototypes ---------- */
void initMenu(void);
void addMenuItem(const char *name, float price, const char *category);
void clearInputBuffer(void);
int  getIntInput(const char *prompt);
float getFloatInput(const char *prompt);

void printHeader(const char *title);
void displayMenu(void);
void displayMenuByCategory(const char *category);

void addToOrder(void);
void viewOrder(void);
void removeFromOrder(void);
float calculateSubtotal(void);
void checkout(void);

MenuItem* findMenuItemById(int id);
void mainMenuLoop(void);

/* ==================================================== */
/*                        MAIN                          */
/* ==================================================== */
int main(void) {
    initMenu();
    printf("=========================================\n");
    printf("   Welcome to The Console Bistro!\n");
    printf("=========================================\n");
    mainMenuLoop();
    printf("\nThank you for visiting The Console Bistro. Goodbye!\n");
    return 0;
}

/* ==================================================== */
/*                  MENU INITIALIZATION                 */
/* ==================================================== */
void initMenu(void) {
    /* Appetizers */
    addMenuItem("Garlic Bread",        4.99f, "Appetizer");
    addMenuItem("Mozzarella Sticks",   6.49f, "Appetizer");
    addMenuItem("Caesar Salad",        7.25f, "Appetizer");

    /* Main Courses */
    addMenuItem("Grilled Chicken",    14.99f, "Main");
    addMenuItem("Cheeseburger",       11.50f, "Main");
    addMenuItem("Spaghetti Bolognese",13.75f, "Main");
    addMenuItem("Margherita Pizza",   12.99f, "Main");
    addMenuItem("Grilled Salmon",     17.95f, "Main");

    /* Beverages */
    addMenuItem("Soft Drink",          2.50f, "Beverage");
    addMenuItem("Iced Tea",            2.75f, "Beverage");
    addMenuItem("Coffee",              3.00f, "Beverage");
    addMenuItem("Lemonade",            2.95f, "Beverage");

    /* Desserts */
    addMenuItem("Chocolate Cake",      5.99f, "Dessert");
    addMenuItem("Cheesecake",          6.25f, "Dessert");
    addMenuItem("Ice Cream Sundae",    4.50f, "Dessert");
}

void addMenuItem(const char *name, float price, const char *category) {
    if (menuCount >= MAX_MENU_ITEMS) return;
    menu[menuCount].id = menuCount + 1;
    strncpy(menu[menuCount].name, name, NAME_LEN - 1);
    menu[menuCount].name[NAME_LEN - 1] = '\0';
    menu[menuCount].price = price;
    strncpy(menu[menuCount].category, category, sizeof(menu[menuCount].category) - 1);
    menu[menuCount].category[sizeof(menu[menuCount].category) - 1] = '\0';
    menuCount++;
}

/* ==================================================== */
/*                    INPUT HELPERS                     */
/* ==================================================== */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

int getIntInput(const char *prompt) {
    int value;
    char line[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) != NULL) {
            if (sscanf(line, "%d", &value) == 1) {
                return value;
            }
        }
        printf("  Invalid input. Please enter a whole number.\n");
    }
}

float getFloatInput(const char *prompt) {
    float value;
    char line[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) != NULL) {
            if (sscanf(line, "%f", &value) == 1) {
                return value;
            }
        }
        printf("  Invalid input. Please enter a number.\n");
    }
}

/* ==================================================== */
/*                     DISPLAY HELPERS                  */
/* ==================================================== */
void printHeader(const char *title) {
    printf("\n-----------------------------------------\n");
    printf(" %s\n", title);
    printf("-----------------------------------------\n");
}

void displayMenu(void) {
    printHeader("FULL MENU");
    const char *categories[] = {"Appetizer", "Main", "Beverage", "Dessert"};
    int numCategories = 4;

    for (int c = 0; c < numCategories; c++) {
        displayMenuByCategory(categories[c]);
    }
    printf("-----------------------------------------\n");
}

void displayMenuByCategory(const char *category) {
    printf("\n[%s]\n", category);
    for (int i = 0; i < menuCount; i++) {
        if (strcmp(menu[i].category, category) == 0) {
            printf("  %2d. %-25s $%.2f\n", menu[i].id, menu[i].name, menu[i].price);
        }
    }
}

/* ==================================================== */
/*                    ORDER MANAGEMENT                  */
/* ==================================================== */
MenuItem* findMenuItemById(int id) {
    for (int i = 0; i < menuCount; i++) {
        if (menu[i].id == id) {
            return &menu[i];
        }
    }
    return NULL;
}

void addToOrder(void) {
    displayMenu();
    int id = getIntInput("\nEnter the item number to add (0 to cancel): ");
    if (id == 0) return;

    MenuItem *item = findMenuItemById(id);
    if (item == NULL) {
        printf("  No such item. Please try again.\n");
        return;
    }

    int qty = getIntInput("Enter quantity: ");
    if (qty <= 0) {
        printf("  Quantity must be positive.\n");
        return;
    }

    /* If item already in order, just increase quantity */
    for (int i = 0; i < orderCount; i++) {
        if (order[i].menuId == id) {
            order[i].quantity += qty;
            printf("  Updated %s quantity to %d.\n", item->name, order[i].quantity);
            return;
        }
    }

    if (orderCount >= MAX_ORDER_ITEMS) {
        printf("  Order is full, cannot add more distinct items.\n");
        return;
    }

    order[orderCount].menuId = id;
    order[orderCount].quantity = qty;
    orderCount++;
    printf("  Added %d x %s to your order.\n", qty, item->name);
}

void viewOrder(void) {
    printHeader("YOUR CURRENT ORDER");
    if (orderCount == 0) {
        printf("  (empty)\n");
        return;
    }

    printf("  %-3s %-25s %-5s %-10s\n", "#", "Item", "Qty", "Line Total");
    for (int i = 0; i < orderCount; i++) {
        MenuItem *item = findMenuItemById(order[i].menuId);
        if (item != NULL) {
            float lineTotal = item->price * order[i].quantity;
            printf("  %-3d %-25s %-5d $%-9.2f\n", i + 1, item->name, order[i].quantity, lineTotal);
        }
    }

    float subtotal = calculateSubtotal();
    float tax = subtotal * TAX_RATE;
    float total = subtotal + tax;

    printf("-----------------------------------------\n");
    printf("  Subtotal: $%.2f\n", subtotal);
    printf("  Tax (%.0f%%): $%.2f\n", TAX_RATE * 100, tax);
    printf("  Total:    $%.2f\n", total);
}

void removeFromOrder(void) {
    if (orderCount == 0) {
        printf("  Your order is empty, nothing to remove.\n");
        return;
    }

    viewOrder();
    int lineNum = getIntInput("\nEnter the line number to remove (0 to cancel): ");
    if (lineNum == 0) return;

    if (lineNum < 1 || lineNum > orderCount) {
        printf("  Invalid line number.\n");
        return;
    }

    int idx = lineNum - 1;
    MenuItem *item = findMenuItemById(order[idx].menuId);
    printf("  Removed %s from your order.\n", item ? item->name : "item");

    for (int i = idx; i < orderCount - 1; i++) {
        order[i] = order[i + 1];
    }
    orderCount--;
}

float calculateSubtotal(void) {
    float subtotal = 0.0f;
    for (int i = 0; i < orderCount; i++) {
        MenuItem *item = findMenuItemById(order[i].menuId);
        if (item != NULL) {
            subtotal += item->price * order[i].quantity;
        }
    }
    return subtotal;
}

void checkout(void) {
    if (orderCount == 0) {
        printf("  Your order is empty. Add some items first!\n");
        return;
    }

    printHeader("RECEIPT - The Console Bistro");
    for (int i = 0; i < orderCount; i++) {
        MenuItem *item = findMenuItemById(order[i].menuId);
        if (item != NULL) {
            float lineTotal = item->price * order[i].quantity;
            printf("  %-25s x%-3d $%.2f\n", item->name, order[i].quantity, lineTotal);
        }
    }

    float subtotal = calculateSubtotal();
    float tax = subtotal * TAX_RATE;
    float total = subtotal + tax;

    printf("-----------------------------------------\n");
    printf("  Subtotal: $%.2f\n", subtotal);
    printf("  Tax:      $%.2f\n", tax);
    printf("  TOTAL:    $%.2f\n", total);
    printf("-----------------------------------------\n");

    char confirm[10];
    printf("\nConfirm order? (y/n): ");
    fgets(confirm, sizeof(confirm), stdin);

    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        printf("\n  Order placed! Your food will be ready shortly.\n");
        orderCount = 0; /* reset order after checkout */
    } else {
        printf("\n  Checkout cancelled. Your order is still saved.\n");
    }
}

/* ==================================================== */
/*                     MAIN MENU LOOP                    */
/* ==================================================== */
void mainMenuLoop(void) {
    int choice;
    do {
        printf("\n=========================================\n");
        printf("  1. View Menu\n");
        printf("  2. Add Item to Order\n");
        printf("  3. View Current Order\n");
        printf("  4. Remove Item from Order\n");
        printf("  5. Checkout\n");
        printf("  6. Exit\n");
        printf("=========================================\n");

        choice = getIntInput("Choose an option: ");

        switch (choice) {
            case 1: displayMenu();       break;
            case 2: addToOrder();        break;
            case 3: viewOrder();         break;
            case 4: removeFromOrder();   break;
            case 5: checkout();          break;
            case 6: printf("\nExiting...\n"); break;
            default: printf("  Invalid option, please choose 1-6.\n");
        }
    } while (choice != 6);
}