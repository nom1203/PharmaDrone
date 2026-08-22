/*
 * Campus Food Delivery App
 * ------------------------
 * Features:
 *   - Select a restaurant on campus
 *   - Browse the restaurant menu
 *   - Add items into a cart before checkout
 *   - View the delivery route as an animated visual
 *
 * Compile: gcc menu.c -o menu
 * Run:     ./menu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_RESTAURANTS      6
#define MAX_ITEMS_PER_REST   12
#define MAX_CART_ITEMS       20
#define NAME_LEN             40
#define TAX_RATE             0.08f

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
    char category[20];
} MenuItem;

typedef struct {
    int id;
    char name[NAME_LEN];
    char cuisine[NAME_LEN];
    MenuItem items[MAX_ITEMS_PER_REST];
    int itemCount;
} Restaurant;

typedef struct {
    int restaurantId;
    int menuId;
    int quantity;
} CartItem;

Restaurant restaurants[MAX_RESTAURANTS];
int restaurantCount = 0;
int selectedRestaurantIndex = -1;
CartItem cart[MAX_CART_ITEMS];
int cartCount = 0;

void delayMs(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

/* Clears the terminal screen. Uses the platform's native clear command,
 * which is far more reliably supported across terminals, IDE consoles,
 * and older Windows setups than raw ANSI escape codes. */
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    fflush(stdout);
}

void printHeader(const char *title) {
    printf("\n========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
}

void pauseForUser(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int getIntInput(const char *prompt) {
    int value;
    char line[64];

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) != NULL) {
           if (sscanf(line, "%d", &value) == 1) {
               return value;
           }
        }
        printf("  Invalid input. Please enter a whole number.\n");
        clearInputBuffer();
    }
}

void addRestaurant(const char *name, const char *cuisine) {
    if (restaurantCount >= MAX_RESTAURANTS) {
        return;
    }

    Restaurant *restaurant = &restaurants[restaurantCount];
    restaurant->id = restaurantCount + 1;
    snprintf(restaurant->name, sizeof(restaurant->name), "%s", name);
    snprintf(restaurant->cuisine, sizeof(restaurant->cuisine), "%s", cuisine);
    restaurant->itemCount = 0;
    restaurantCount++;
}

void addMenuItemToRestaurant(int restaurantIndex, int id, const char *name, float price, const char *category) {
    if (restaurantIndex < 0 || restaurantIndex >= restaurantCount) {
        return;
    }

    Restaurant *restaurant = &restaurants[restaurantIndex];
    if (restaurant->itemCount >= MAX_ITEMS_PER_REST) {
        return;
    }

    MenuItem *item = &restaurant->items[restaurant->itemCount];
    item->id = id;
    snprintf(item->name, sizeof(item->name), "%s", name);
    item->price = price;
    snprintf(item->category, sizeof(item->category), "%s", category);
    restaurant->itemCount++;
}

MenuItem *findMenuItemInRestaurant(Restaurant *restaurant, int itemId) {
    if (restaurant == NULL) {
        return NULL;
    }

    for (int i = 0; i < restaurant->itemCount; i++) {
        if (restaurant->items[i].id == itemId) {
           return &restaurant->items[i];
        }
    }
    return NULL;
}

void loadRestaurants(void) {
    addRestaurant("North Hall Cafe", "Campus Grill");
    addMenuItemToRestaurant(0, 1, "Veggie Wrap", 9.50f, "Main");
    addMenuItemToRestaurant(0, 2, "Chicken Burger", 11.00f, "Main");
    addMenuItemToRestaurant(0, 3, "French Fries", 4.25f, "Side");
    addMenuItemToRestaurant(0, 4, "Iced Coffee", 3.75f, "Drink");
    addMenuItemToRestaurant(0, 5, "Brownie", 3.50f, "Dessert");

    addRestaurant("Zen Noodle Bar", "Asian");
    addMenuItemToRestaurant(1, 1, "Teriyaki Bowl", 12.50f, "Main");
    addMenuItemToRestaurant(1, 2, "Spicy Ramen", 11.75f, "Main");
    addMenuItemToRestaurant(1, 3, "Edamame", 4.50f, "Side");
    addMenuItemToRestaurant(1, 4, "Green Tea", 2.95f, "Drink");
    addMenuItemToRestaurant(1, 5, "Mochi", 3.25f, "Dessert");

    addRestaurant("Sunset Pizza Co.", "Italian");
    addMenuItemToRestaurant(2, 1, "Margherita Pizza", 13.00f, "Main");
    addMenuItemToRestaurant(2, 2, "Pepperoni Slice", 4.25f, "Main");
    addMenuItemToRestaurant(2, 3, "Garlic Bread", 5.50f, "Side");
    addMenuItemToRestaurant(2, 4, "Lemon Soda", 2.80f, "Drink");
    addMenuItemToRestaurant(2, 5, "Tiramisu", 4.75f, "Dessert");

    addRestaurant("Green Bowl", "Healthy");
    addMenuItemToRestaurant(3, 1, "Avocado Salad", 10.25f, "Main");
    addMenuItemToRestaurant(3, 2, "Quinoa Power Bowl", 12.95f, "Main");
    addMenuItemToRestaurant(3, 3, "Fruit Cup", 5.00f, "Side");
    addMenuItemToRestaurant(3, 4, "Sparkling Water", 2.00f, "Drink");
    addMenuItemToRestaurant(3, 5, "Protein Cookie", 3.80f, "Dessert");
}

void displayRestaurants(void) {
    printHeader("CHOOSE A RESTAURANT");
    for (int i = 0; i < restaurantCount; i++) {
        printf("  %d. %-22s (%s)\n", restaurants[i].id, restaurants[i].name, restaurants[i].cuisine);
    }
}

Restaurant *selectRestaurant(void) {
    displayRestaurants();
    int choice = getIntInput("\nSelect a restaurant (0 to cancel): ");

    if (choice == 0) {
        return NULL;
    }

    if (choice < 1 || choice > restaurantCount) {
        printf("  Invalid restaurant selection.\n");
        return NULL;
    }

    selectedRestaurantIndex = choice - 1;
    return &restaurants[selectedRestaurantIndex];
}

void displayMenuForRestaurant(Restaurant *restaurant) {
    if (restaurant == NULL) {
        printf("  Please select a restaurant first.\n");
        return;
    }

    printHeader("MENU FOR THE SELECTED RESTAURANT");
    printf("  %s (%s)\n\n", restaurant->name, restaurant->cuisine);

    for (int i = 0; i < restaurant->itemCount; i++) {
        MenuItem *item = &restaurant->items[i];
        printf("  %2d. %-24s $%.2f   [%s]\n", item->id, item->name, item->price, item->category);
    }
}

void addToCart(void) {
    Restaurant *restaurant = NULL;

    if (selectedRestaurantIndex == -1) {
        restaurant = selectRestaurant();
    } else {
        restaurant = &restaurants[selectedRestaurantIndex];
    }

    if (restaurant == NULL) {
        return;
    }

    displayMenuForRestaurant(restaurant);
    int itemId = getIntInput("\nEnter item number to add (0 to cancel): ");
    if (itemId == 0) {
        return;
    }

    MenuItem *item = findMenuItemInRestaurant(restaurant, itemId);
    if (item == NULL) {
        printf("  Item not found in this restaurant's menu.\n");
        return;
    }

    int quantity = getIntInput("Enter quantity: ");
    if (quantity <= 0) {
        printf("  Quantity must be greater than zero.\n");
        return;
    }

    for (int i = 0; i < cartCount; i++) {
        if (cart[i].restaurantId == restaurant->id && cart[i].menuId == itemId) {
           cart[i].quantity += quantity;
           printf("  Updated cart: %d x %s\n", cart[i].quantity, item->name);
           return;
        }
    }

    if (cartCount >= MAX_CART_ITEMS) {
        printf("  Cart is full. You cannot add more items.\n");
        return;
    }

    cart[cartCount].restaurantId = restaurant->id;
    cart[cartCount].menuId = itemId;
    cart[cartCount].quantity = quantity;
    cartCount++;

    printf("  Added %d x %s to your cart.\n", quantity, item->name);
}

float calculateCartSubtotal(void) {
    float subtotal = 0.0f;

    for (int i = 0; i < cartCount; i++) {
        Restaurant *restaurant = NULL;
        for (int r = 0; r < restaurantCount; r++) {
           if (restaurants[r].id == cart[i].restaurantId) {
               restaurant = &restaurants[r];
               break;
           }
        }

        if (restaurant != NULL) {
           MenuItem *item = findMenuItemInRestaurant(restaurant, cart[i].menuId);
           if (item != NULL) {
               subtotal += item->price * cart[i].quantity;
           }
        }
    }

    return subtotal;
}

void viewCart(void) {
    printHeader("YOUR CART");

    if (cartCount == 0) {
        printf("  Your cart is empty.\n");
        return;
    }

    float subtotal = 0.0f;
    printf("  %-3s %-20s %-8s %-10s\n", "#", "Item", "Qty", "Total");

    for (int i = 0; i < cartCount; i++) {
        Restaurant *restaurant = NULL;
        for (int r = 0; r < restaurantCount; r++) {
           if (restaurants[r].id == cart[i].restaurantId) {
               restaurant = &restaurants[r];
               break;
           }
        }

        if (restaurant == NULL) {
           continue;
        }

        MenuItem *item = findMenuItemInRestaurant(restaurant, cart[i].menuId);
        if (item != NULL) {
           float lineTotal = item->price * cart[i].quantity;
           subtotal += lineTotal;
           printf("  %-3d %-20s %-8d $%.2f\n", i + 1, item->name, cart[i].quantity, lineTotal);
        }
    }

    float tax = subtotal * TAX_RATE;
    float total = subtotal + tax;

    printf("  --------------------------------------\n");
    printf("  Subtotal: $%.2f\n", subtotal);
    printf("  Tax:      $%.2f\n", tax);
    printf("  Total:    $%.2f\n", total);
}

void removeFromCart(void) {
    if (cartCount == 0) {
        printf("  Cart is already empty.\n");
        return;
    }

    viewCart();
    int index = getIntInput("\nEnter cart item number to remove (0 to cancel): ");
    if (index == 0) {
        return;
    }

    if (index < 1 || index > cartCount) {
        printf("  Invalid cart item number.\n");
        return;
    }

    for (int i = index - 1; i < cartCount - 1; i++) {
        cart[i] = cart[i + 1];
    }
    cartCount--;
    printf("  Item removed from cart.\n");
}

void showDeliveryVisual(void) {
    const char *stages[] = {
        "Restaurant pickup",
        "Leaving campus gate",
        "Crossing library path",
        "Approaching dorms",
        "Arriving at your building",
        "Delivered"
    };

    for (int i = 0; i < 6; i++) {
        clearScreen();
        printHeader("DELIVERY TRACKER");
        printf("\n  %s\n", stages[i]);
        printf("     [D] ----> [X] ----> [Y] ----> [YOU]\n");
        fflush(stdout);
        delayMs(550);
    }

    clearScreen();
    printHeader("DELIVERY TRACKER");
    printf("\n  !! Your campus food delivery has arrived !!\n");
    printf("      ______\n");
    printf("     /     \\\n");
    printf("     | [ ] |\n");
    printf("     |  o  |\n");
    printf("      \\___/\n");
    printf("\n  Enjoy your meal!\n");
}

void checkout(void) {
    if (cartCount == 0) {
        printf("  Cart is empty. Add items before checkout.\n");
        return;
    }

    viewCart();
    char answer[8];
    printf("\nConfirm order? (y/n): ");
    if (fgets(answer, sizeof(answer), stdin) == NULL) {
        return;
    }

    if (answer[0] != 'y' && answer[0] != 'Y') {
        printf("  Checkout cancelled. Your cart is still saved.\n");
        return;
    }

    float subtotal = calculateCartSubtotal();
    float tax = subtotal * TAX_RATE;
    float total = subtotal + tax;

    printf("\nOrder confirmed!\n");
    printf("  Subtotal: $%.2f\n", subtotal);
    printf("  Tax:      $%.2f\n", tax);
    printf("  Total:    $%.2f\n", total);
    printf("  Estimated delivery: 15-20 minutes\n");

    cartCount = 0;
    selectedRestaurantIndex = -1;
    showDeliveryVisual();
}

void mainMenuLoop(void) {
    int choice = 0;

    while (choice != 7) {
        clearScreen();
        printf("\n========================================\n");
        printf("  Campus Food Delivery App\n");
        printf("  1. Pick a restaurant\n");
        printf("  2. View selected restaurant menu\n");
        printf("  3. Add food to cart\n");
        printf("  4. View cart\n");
        printf("  5. Remove item from cart\n");
        printf("  6. Checkout\n");
        printf("  7. Exit\n");
        printf("========================================\n");

        choice = getIntInput("Choose an option: ");

        switch (choice) {
           case 1:
               selectRestaurant();
               pauseForUser();
               break;
           case 2:
               if (selectedRestaurantIndex == -1) {
                   printf("  No restaurant selected yet.\n");
               } else {
                   displayMenuForRestaurant(&restaurants[selectedRestaurantIndex]);
               }
               pauseForUser();
               break;
           case 3:
               addToCart();
               pauseForUser();
               break;
           case 4:
               viewCart();
               pauseForUser();
               break;
           case 5:
               removeFromCart();
               pauseForUser();
               break;
           case 6:
               checkout();
               pauseForUser();
               break;
           case 7:
               clearScreen();
               printf("\nLeaving campus food delivery app. Goodbye!\n");
               break;
           default:
               printf("  Invalid option. Please choose between 1 and 7.\n");
               pauseForUser();
               break;
        }
    }
}

int main(void) {
    loadRestaurants();
    printf("========================================\n");
    printf("  Welcome to Campus Food Delivery\n");
    printf("========================================\n");
    printf("  Pick a restaurant, build your cart, and track your delivery.\n");
    mainMenuLoop();
    return 0;
}
