/* ============================================================
   pharmacy.c
   Module owner: [Member A]

   Holds the pharmacy catalog and handles searching it. The data
   is hardcoded for now, but everything goes through the
   functions in pharmacy.h, so this could later be swapped to
   read from a file or database without changing other modules.
   ============================================================ */

#include <string.h>
#include <stdio.h>

#include "../include/pharmacy.h"
#include "../include/utils.h"

static Pharmacy pharmacies[MAX_PHARMACIES];
static int      pharmacyCount = 0;

 
/* Adds one medicine entry to a pharmacy's medicine list. */
static void addMed(Pharmacy *p, const char *name, float price, int stock) {
    if (p->medCount >= MAX_MEDS_PER_PHARMACY) return;
    strncpy(p->meds[p->medCount].name, name, MAX_NAME_LEN - 1);
    p->meds[p->medCount].name[MAX_NAME_LEN - 1] = '\0';
    p->meds[p->medCount].price = price;
    p->meds[p->medCount].stock = stock;
    p->medCount++;
}
 
/* Fills the pharmacy list with sample data. Must be called once
   before any other pharmacy_* function is used. */
void pharmacy_init(void) {
    pharmacyCount = 0;

    strcpy(pharmacies[pharmacyCount].name, "GreenLeaf Pharmacy");
    strcpy(pharmacies[pharmacyCount].address, "12 Orchard Road");
    pharmacies[pharmacyCount].etaMinutes = 8.0;
    pharmacies[pharmacyCount].medCount = 0;
    addMed(&pharmacies[pharmacyCount], "Paracetamol", 3.50, 120);
    addMed(&pharmacies[pharmacyCount], "Amoxicillin", 8.75, 40);
    addMed(&pharmacies[pharmacyCount], "Metformin",   6.20, 75);
    addMed(&pharmacies[pharmacyCount], "Ibuprofen",   4.10, 90);
    pharmacyCount++;

    strcpy(pharmacies[pharmacyCount].name, "CarePlus Chemist");
    strcpy(pharmacies[pharmacyCount].address, "45 Marina Boulevard");
    pharmacies[pharmacyCount].etaMinutes = 12.5;
    pharmacies[pharmacyCount].medCount = 0;
    addMed(&pharmacies[pharmacyCount], "Paracetamol",  3.20, 60);
    addMed(&pharmacies[pharmacyCount], "Atorvastatin", 11.00, 25);
    addMed(&pharmacies[pharmacyCount], "Amoxicillin",  9.10, 0);   /* out of stock */
    addMed(&pharmacies[pharmacyCount], "Losartan",     7.40, 55);
    pharmacyCount++;

    strcpy(pharmacies[pharmacyCount].name, "MediWell Pharmacy");
    strcpy(pharmacies[pharmacyCount].address, "8 Raffles Avenue");
    pharmacies[pharmacyCount].etaMinutes = 6.0;
    pharmacies[pharmacyCount].medCount = 0;
    addMed(&pharmacies[pharmacyCount], "Metformin",   5.90, 100);
    addMed(&pharmacies[pharmacyCount], "Ibuprofen",   3.95, 15);
    addMed(&pharmacies[pharmacyCount], "Cetirizine",  2.80, 70);
    addMed(&pharmacies[pharmacyCount], "Amoxicillin", 8.30, 90);
    pharmacyCount++;

    strcpy(pharmacies[pharmacyCount].name, "HealthHub Pharmacy");
    strcpy(pharmacies[pharmacyCount].address, "101 Bugis Street");
    pharmacies[pharmacyCount].etaMinutes = 15.0;
    pharmacies[pharmacyCount].medCount = 0;
    addMed(&pharmacies[pharmacyCount], "Losartan",     7.10, 30);
    addMed(&pharmacies[pharmacyCount], "Atorvastatin", 10.50, 45);
    addMed(&pharmacies[pharmacyCount], "Paracetamol",  3.80, 200);
    addMed(&pharmacies[pharmacyCount], "Cetirizine",   3.00, 20);
    pharmacyCount++;

    strcpy(pharmacies[pharmacyCount].name, "QuickCare Drugstore");
    strcpy(pharmacies[pharmacyCount].address, "77 Clementi Ave");
    pharmacies[pharmacyCount].etaMinutes = 9.5;
    pharmacies[pharmacyCount].medCount = 0;
    addMed(&pharmacies[pharmacyCount], "Ibuprofen",   4.50, 55);
    addMed(&pharmacies[pharmacyCount], "Metformin",   6.00, 0);   /* out of stock */
    addMed(&pharmacies[pharmacyCount], "Amoxicillin", 8.60, 35);
    addMed(&pharmacies[pharmacyCount], "Cetirizine",  2.95, 60);
    pharmacyCount++;
}
 
/* Returns how many pharmacies are in the catalog. */
int pharmacy_get_count(void) {
    return pharmacyCount;
}
 
/* Returns a pointer to the pharmacy at this index, or NULL if
   the index is out of range. */
const Pharmacy *pharmacy_get(int index) {
    if (index < 0 || index >= pharmacyCount) return NULL;
    return &pharmacies[index];
}
 
/* Finds every pharmacy that has medName in stock (case-insensitive).
   Fills matchIndices[] with their indices and returns how many
   matches were found. Pharmacies with 0 stock are skipped. */
int pharmacy_find_matches(const char *medName, int matchIndices[]) {
    char medLower[MAX_NAME_LEN];
    util_to_lower(medName, medLower);
    int matches = 0;

    for (int p = 0; p < pharmacyCount; p++) {
        for (int m = 0; m < pharmacies[p].medCount; m++) {
            char stockLower[MAX_NAME_LEN];
            util_to_lower(pharmacies[p].meds[m].name, stockLower);
            if (strcmp(medLower, stockLower) == 0 && pharmacies[p].meds[m].stock > 0) {
                matchIndices[matches++] = p;
                break;
            }
        }
    }
    return matches;
}
 
/* Looks up medName at the given pharmacy (case-insensitive).
   If found, copies its price/stock into *out and returns 1.
   Otherwise returns 0. */
int pharmacy_get_medication(int pharmacyIndex, const char *medName, Medication *out) {
    if (pharmacyIndex < 0 || pharmacyIndex >= pharmacyCount) return 0;

    char targetLower[MAX_NAME_LEN];
    util_to_lower(medName, targetLower);

    const Pharmacy *p = &pharmacies[pharmacyIndex];
    for (int m = 0; m < p->medCount; m++) {
        char candidateLower[MAX_NAME_LEN];
        util_to_lower(p->meds[m].name, candidateLower);
        if (strcmp(candidateLower, targetLower) == 0) {
            *out = p->meds[m];
            return 1;
        }
    }
    return 0;
}
 
/* Prints every pharmacy and its full medicine list, including
   anything that's out of stock. */
void pharmacy_print_catalog(void) {
    util_print_divider();
    printf("ALL PHARMACIES & STOCK\n");
    util_print_divider();
    for (int p = 0; p < pharmacyCount; p++) {
        printf("%s - %s (ETA %.1f min)\n",
               pharmacies[p].name, pharmacies[p].address, pharmacies[p].etaMinutes);
        for (int m = 0; m < pharmacies[p].medCount; m++) {
            printf("    %-16s $%-6.2f stock: %d%s\n",
                   pharmacies[p].meds[m].name,
                   pharmacies[p].meds[m].price,
                   pharmacies[p].meds[m].stock,
                   pharmacies[p].meds[m].stock == 0 ? "  (out of stock)" : "");
        }
        printf("\n");
    }
}
