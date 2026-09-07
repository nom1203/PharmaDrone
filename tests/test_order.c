/* ============================================================
   test_order.c
   ------------------------------------------------------------
   Unit tests for the order module. order_run_flow() and
   order_dispatch_drone() are interactive/console-driven, so
   they aren't unit tested directly here -- instead we test
   order_calculate_total(), the pure logic extracted specifically
   so it CAN be tested without simulating keyboard input.
   ============================================================ */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/order.h"

static void test_total_of_multiple_items(void) {
    OrderItem orders[2];

    strcpy(orders[0].medicineName, "Paracetamol");
    orders[0].pharmacyIndex = 0;
    orders[0].price = 3.50f;

    strcpy(orders[1].medicineName, "Amoxicillin");
    orders[1].pharmacyIndex = 2;
    orders[1].price = 8.30f;

    float total = order_calculate_total(orders, 2);
    assert(total > 11.79f && total < 11.81f);
    printf("  [OK] order_calculate_total sums multiple items correctly\n");
}

static void test_total_of_zero_items(void) {
    OrderItem orders[1] = { {"", 0, 0.0f} };
    float total = order_calculate_total(orders, 0);
    assert(total == 0.0f);
    printf("  [OK] order_calculate_total returns 0 for an empty order\n");
}

static void test_total_of_single_item(void) {
    OrderItem orders[1];
    strcpy(orders[0].medicineName, "Ibuprofen");
    orders[0].pharmacyIndex = 1;
    orders[0].price = 4.10f;

    float total = order_calculate_total(orders, 1);
    assert(total > 4.09f && total < 4.11f);
    printf("  [OK] order_calculate_total handles a single item\n");
}

int main(void) {
    printf("Running order module unit tests...\n");

    test_total_of_multiple_items();
    test_total_of_zero_items();
    test_total_of_single_item();

    printf("test_order: ALL TESTS PASSED\n");
    return 0;
}
