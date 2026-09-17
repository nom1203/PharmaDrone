/* ============================================================
   main.c
   ------------------------------------------------------------
   Shared / integration file (all team members).

    just sets up the pharmacy data and runs the main menu,
    calling into the pharmacy and order modules for the actual work.
   ============================================================ */

#include <stdio.h>
#include <string.h>

#include "../include/pharmacy.h"
#include "../include/order.h"
#include "../include/utils.h"

/* Prints the title banner shown once at startup. */
static void printBanner(void) {
    printf("============================================================\n");
    printf("           PharmaDrone - Prescription Delivery              \n");
    printf("============================================================\n");
}

int main(void) {
    /* Try loading the pharmacy catalog from CSV first; if it's
       missing or empty, fall back to the built-in sample data so
       the app still runs out of the box. */
    if (!pharmacy_load_from_csv("data/pharmacies.csv")) {
        printf("(No data/pharmacies.csv found -- using built-in sample data.)\n");
        pharmacy_init();
    }
    printBanner();

    int running = 1;
    while (running) {
        printf("\nMAIN MENU\n");
        printf("  1. Upload prescription & order drone delivery\n");
        printf("  2. Browse all pharmacies & stock\n");
        printf("  3. Exit\n");
        printf("Choice: ");

        char choice[8];
        if (!fgets(choice, sizeof(choice), stdin)) break;
        util_trim(choice);

        if (strcmp(choice, "1") == 0) {
            printf("\n");
            order_run_flow();
        } else if (strcmp(choice, "2") == 0) {
            printf("\n");
            pharmacy_print_catalog();
        } else if (strcmp(choice, "3") == 0) {
            printf("\nThank you for using PharmaDrone. Stay healthy!\n");
            running = 0;
        } else {
            printf("\nInvalid choice, please try again.\n");
        }
    }

    return 0;
}