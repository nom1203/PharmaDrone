/* ============================================================
   test_pharmacy.c
   ------------------------------------------------------------
   Unit tests for the pharmacy module, tested on its own (no
   prescription/order/main code linked in).
   ============================================================ */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/pharmacy.h"
 
/* Sample data should load all 5 pharmacies. */
static void test_init_loads_pharmacies(void) {
    pharmacy_init();
    assert(pharmacy_get_count() == 5);
    printf("  [OK] pharmacy_init loads 5 pharmacies\n");
}
 
/* pharmacy_get should return NULL for out-of-range indices,
   not crash or read garbage memory. */
static void test_get_bounds(void) {
    assert(pharmacy_get(0) != NULL);
    assert(pharmacy_get(pharmacy_get_count() - 1) != NULL);
    assert(pharmacy_get(-1) == NULL);
    assert(pharmacy_get(pharmacy_get_count()) == NULL);
    printf("  [OK] pharmacy_get respects array bounds\n");
}
 
/* Paracetamol is in stock at 3 pharmacies (GreenLeaf, CarePlus,
   HealthHub) -- also checks the search is case-insensitive. */
static void test_find_matches_common_drug(void) {
    int matches[MAX_MATCHES];
    
    int n = pharmacy_find_matches("paracetamol", matches); /* lowercase on purpose */
    assert(n == 3);
    printf("  [OK] pharmacy_find_matches is case-insensitive and finds 3 matches for Paracetamol\n");
}
 
/* Amoxicillin exists at 4 pharmacies but CarePlus has 0 stock,
   so it should only match the other 3. */
static void test_find_matches_excludes_out_of_stock(void) {
    int matches[MAX_MATCHES];
    /* Amoxicillin exists at 4 pharmacies but CarePlus has 0 stock -> should be excluded */
    int n = pharmacy_find_matches("Amoxicillin", matches);
    assert(n == 3);
    printf("  [OK] pharmacy_find_matches excludes out-of-stock pharmacies\n");
}

/* A medicine no pharmacy carries should return 0 matches. */
static void test_find_matches_unknown_drug(void) {
    int matches[MAX_MATCHES];
    int n = pharmacy_find_matches("NotARealDrug", matches);
    assert(n == 0);
    printf("  [OK] pharmacy_find_matches returns 0 for an unknown medicine\n");
}
 
/* Should find a medicine that exists and correctly report one
   that doesn't, without mixing the two up. */
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
