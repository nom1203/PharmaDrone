/* ============================================================
   test_pharmacy.c
   ------------------------------------------------------------
   Unit tests for the pharmacy module, tested in isolation from
   the rest of the program (no prescription/order/main code is
   linked in). Run with `make test` or directly after building
   with the pharmacy test target.

   Each test prints what it checked, so a failed assert() shows
   exactly which line/condition broke.
   ============================================================ */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/pharmacy.h"

static void test_init_loads_pharmacies(void) {
    pharmacy_init();
    assert(pharmacy_get_count() == 5);
    printf("  [OK] pharmacy_init loads 5 pharmacies\n");
}

static void test_get_bounds(void) {
    assert(pharmacy_get(0) != NULL);
    assert(pharmacy_get(pharmacy_get_count() - 1) != NULL);
    assert(pharmacy_get(-1) == NULL);
    assert(pharmacy_get(pharmacy_get_count()) == NULL);
    printf("  [OK] pharmacy_get respects array bounds\n");
}

static void test_find_matches_common_drug(void) {
    int matches[MAX_MATCHES];
    /* Paracetamol is in stock at 3 pharmacies (GreenLeaf, CarePlus, HealthHub) */
    int n = pharmacy_find_matches("paracetamol", matches); /* lowercase on purpose */
    assert(n == 3);
    printf("  [OK] pharmacy_find_matches is case-insensitive and finds 3 matches for Paracetamol\n");
}

static void test_find_matches_excludes_out_of_stock(void) {
    int matches[MAX_MATCHES];
    /* Amoxicillin exists at 4 pharmacies but CarePlus has 0 stock -> should be excluded */
    int n = pharmacy_find_matches("Amoxicillin", matches);
    assert(n == 3);
    printf("  [OK] pharmacy_find_matches excludes out-of-stock pharmacies\n");
}

static void test_find_matches_unknown_drug(void) {
    int matches[MAX_MATCHES];
    int n = pharmacy_find_matches("NotARealDrug", matches);
    assert(n == 0);
    printf("  [OK] pharmacy_find_matches returns 0 for an unknown medicine\n");
}

static void test_get_medication_found_and_not_found(void) {
    Medication med;

    int found = pharmacy_get_medication(0, "Paracetamol", &med);
    assert(found == 1);
    assert(med.stock == 120);
    assert(med.price > 3.49f && med.price < 3.51f);

    int notFound = pharmacy_get_medication(0, "SomethingElse", &med);
    assert(notFound == 0);

    printf("  [OK] pharmacy_get_medication finds known medicine and rejects unknown medicine\n");
}

int main(void) {
    printf("Running pharmacy module unit tests...\n");

    test_init_loads_pharmacies();
    test_get_bounds();
    test_find_matches_common_drug();
    test_find_matches_excludes_out_of_stock();
    test_find_matches_unknown_drug();
    test_get_medication_found_and_not_found();

    printf("test_pharmacy: ALL TESTS PASSED\n");
    return 0;
}
