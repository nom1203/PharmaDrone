/* ============================================================
   order.c
   Module owner: [Member C]

   Implements the order-building flow (search pharmacies for
   each prescribed medicine, let the user pick one) and the
   simulated drone pickup/delivery + receipt.
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/order.h"
#include "../include/pharmacy.h"
#include "../include/prescription.h"
#include "../include/utils.h"

float order_calculate_total(const OrderItem orders[], int count) {
    float total = 0.0f;
    for (int i = 0; i < count; i++) {
        total += orders[i].price;
    }
    return total;
}

/* Internal helper: draws one 0-100% progress bar for the drone animation. */
static void printProgressBar(void) {
    for (int bar = 0; bar <= 10; bar++) {
        printf("\r  [");
        for (int b = 0; b < 10; b++) printf(b < bar ? "#" : " ");
        printf("] %d%%", bar * 10);
        fflush(stdout);
        util_sleep_tenths(2);
    }
    printf("\n");
}

void order_dispatch_drone(OrderItem orders[], int orderCount, float totalCost) {
    util_print_divider();
    printf("DRONE DISPATCH\n");
    util_print_divider();

    if (orderCount == 0) {
        printf("No items to deliver.\n");
        return;
    }

    printf("Order confirmed. Preparing drone for pickup...\n");
    util_sleep_tenths(8);

    int visited[MAX_PHARMACIES];
    int visitedCount = 0;

    for (int i = 0; i < orderCount; i++) {
        int p = orders[i].pharmacyIndex;

        int already = 0;
        for (int v = 0; v < visitedCount; v++) if (visited[v] == p) already = 1;
        if (already) continue;
        visited[visitedCount++] = p;

        const Pharmacy *ph = pharmacy_get(p);
        if (!ph) continue;

        printf("\n[Drone] Flying to %s (%s)...\n", ph->name, ph->address);
        printProgressBar();
        printf("[Drone] Arrived at %s.\n", ph->name);

        for (int i2 = 0; i2 < orderCount; i2++) {
            if (orders[i2].pharmacyIndex == p) {
                printf("  -> Picking up %s ($%.2f)\n", orders[i2].medicineName, orders[i2].price);
            }
        }
        util_sleep_tenths(6);
    }

    printf("\n[Drone] All items collected. Heading to delivery address...\n");
    printProgressBar();
    printf("\n[Drone] Package delivered successfully!\n\n");

    util_print_divider();
    printf("ORDER RECEIPT\n");
    util_print_divider();
    for (int i = 0; i < orderCount; i++) {
        const Pharmacy *ph = pharmacy_get(orders[i].pharmacyIndex);
        printf("  %-20s from %-20s  $%6.2f\n",
               orders[i].medicineName,
               ph ? ph->name : "Unknown",
               orders[i].price);
    }
    util_print_divider();
    printf("  TOTAL: $%.2f\n", totalCost);
    util_print_divider();
}

void order_run_flow(void) {
    char prescriptionItems[MAX_PRESCRIPTION_ITEMS][MAX_NAME_LEN];
    int  itemCount = prescription_upload(prescriptionItems);

    if (itemCount == 0) {
        printf("No medications were provided. Returning to main menu.\n\n");
        return;
    }

    OrderItem orders[MAX_PRESCRIPTION_ITEMS];
    int       orderCount = 0;

    for (int i = 0; i < itemCount; i++) {
        util_print_divider();
        printf("Searching pharmacies for: %s\n", prescriptionItems[i]);
        util_print_divider();

        int matchIndices[MAX_MATCHES];
        int matchCount = pharmacy_find_matches(prescriptionItems[i], matchIndices);

        if (matchCount == 0) {
            printf("  No pharmacy currently has \"%s\" in stock. Skipping.\n\n", prescriptionItems[i]);
            continue;
        }

        printf("  %-3s %-22s %-24s %8s %7s %8s\n",
               "#", "Pharmacy", "Address", "Price", "Stock", "ETA(min)");
        for (int m = 0; m < matchCount; m++) {
            int p = matchIndices[m];
            const Pharmacy *ph = pharmacy_get(p);
            Medication med;
            pharmacy_get_medication(p, prescriptionItems[i], &med);
            printf("  %-3d %-22s %-24s %8.2f %7d %8.1f\n",
                   m + 1, ph->name, ph->address, med.price, med.stock, ph->etaMinutes);
        }

        printf("\n  Choose a pharmacy [1-%d], or 0 to skip this medicine: ", matchCount);
        char sel[8];
        if (!fgets(sel, sizeof(sel), stdin)) continue;
        util_trim(sel);
        int choice = atoi(sel);

        if (choice <= 0 || choice > matchCount) {
            printf("  Skipped %s.\n\n", prescriptionItems[i]);
            continue;
        }

        int chosenPharmacy = matchIndices[choice - 1];
        Medication med;
        if (pharmacy_get_medication(chosenPharmacy, prescriptionItems[i], &med)) {
            strncpy(orders[orderCount].medicineName, med.name, MAX_NAME_LEN - 1);
            orders[orderCount].medicineName[MAX_NAME_LEN - 1] = '\0';
            orders[orderCount].pharmacyIndex = chosenPharmacy;
            orders[orderCount].price = med.price;
            orderCount++;
        }
        printf("\n");
    }

    if (orderCount == 0) {
        printf("No items were selected for delivery.\n\n");
        return;
    }

    float totalCost = order_calculate_total(orders, orderCount);

    printf("Ready to dispatch a drone for %d item(s), total $%.2f. Proceed? (y/n): ",
           orderCount, totalCost);
    char confirm[8];
    if (fgets(confirm, sizeof(confirm), stdin)) {
        util_trim(confirm);
        if (confirm[0] == 'y' || confirm[0] == 'Y') {
            order_dispatch_drone(orders, orderCount, totalCost);
        } else {
            printf("Order cancelled.\n\n");
        }
    }
}
