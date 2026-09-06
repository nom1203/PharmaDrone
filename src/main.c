/* ============================================================
   main.c
   ------------------------------------------------------------
   Shared / integration file (all team members).

   This file contains no business logic of its own -- it only
   initialises the pharmacy module and drives the top-level
   menu, delegating each option to the relevant module. This is
   the "integration" step described in the assignment brief:
   each module is developed and unit tested independently, then
   wired together here.
   ============================================================ */

#include <stdio.h>
#include <string.h>

#include "../include/pharmacy.h"
#include "../include/order.h"
#include "../include/utils.h"

static void printBanner(void) {
    printf("============================================================\n");
    printf("           PharmaDrone - Prescription Delivery              \n");
    printf("============================================================\n");
}

int main(void) {
    pharmacy_init();
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
