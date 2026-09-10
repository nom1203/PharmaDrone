/* ============================================================
   pharmacy.c
   Module owner: [Ajlaan]

   Holds the pharmacy catalog and handles searching it. The data
   is hardcoded for now, but everything goes through the
   functions in pharmacy.h, so this could later be swapped to
   read from a file or database without changing other modules.
   ============================================================ */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/pharmacy.h"
#include "../include/utils.h"

#define CSV_LINE_LEN 512
#define CSV_FIELDS   6

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

/* Splits a CSV line into up to maxFields comma-separated fields,
   trimming whitespace from each one. Modifies line in place and
   points fields[] into it. Returns the number of fields found.
   Doesn't handle quoted commas -- not needed for this dataset. */
static int splitCsvLine(char *line, char *fields[], int maxFields) {
    int count = 0;
    char *token = strtok(line, ",");
    while (token != NULL && count < maxFields) {
        util_trim(token);
        fields[count++] = token;
        token = strtok(NULL, ",");
    }
    return count;
}

/* Finds a pharmacy by name in the in-memory list, or creates a
   new one (initialised from name/address/eta) if it isn't there
   yet. Returns its index, or -1 if the catalog is already full. */
static int findOrAddPharmacy(const char *name, const char *address, double etaMinutes) {
    for (int i = 0; i < pharmacyCount; i++) {
        if (strcmp(pharmacies[i].name, name) == 0) return i;
    }
    if (pharmacyCount >= MAX_PHARMACIES) return -1;

    int idx = pharmacyCount++;
    strncpy(pharmacies[idx].name, name, MAX_NAME_LEN - 1);
    pharmacies[idx].name[MAX_NAME_LEN - 1] = '\0';
    strncpy(pharmacies[idx].address, address, MAX_ADDR_LEN - 1);
    pharmacies[idx].address[MAX_ADDR_LEN - 1] = '\0';
    pharmacies[idx].etaMinutes = etaMinutes;
    pharmacies[idx].medCount = 0;
    return idx;
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

/* Loads the pharmacy list from a CSV file (see pharmacy.h for the
   expected column layout), replacing whatever is currently in
   memory. Returns 1 on success, 0 if the file couldn't be opened
   or no valid rows were found. */
int pharmacy_load_from_csv(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    pharmacyCount = 0;

    char line[CSV_LINE_LEN];
    int  lineNo = 0;

    while (fgets(line, sizeof(line), fp)) {
        lineNo++;
        util_trim(line);

        if (line[0] == '\0') continue;   /* skip blank lines */
        if (lineNo == 1) continue;       /* skip the header row */

        char *fields[CSV_FIELDS];
        int   n = splitCsvLine(line, fields, CSV_FIELDS);
        if (n < CSV_FIELDS) {
            fprintf(stderr, "pharmacy_load_from_csv: skipping malformed line %d\n", lineNo);
            continue;
        }

        const char *pharmacyName = fields[0];
        const char *address      = fields[1];
        double      etaMinutes   = atof(fields[2]);
        const char *medName      = fields[3];
        float       price        = (float)atof(fields[4]);
        int         stock        = atoi(fields[5]);

        int idx = findOrAddPharmacy(pharmacyName, address, etaMinutes);
        if (idx == -1) {
            fprintf(stderr, "pharmacy_load_from_csv: too many pharmacies, skipping '%s'\n", pharmacyName);
            continue;
        }
        addMed(&pharmacies[idx], medName, price, stock);
    }

    fclose(fp);
    return pharmacyCount > 0;
}

/* Writes the current in-memory pharmacy list to a CSV file, one
   row per medicine, in the format read by pharmacy_load_from_csv().
   Returns 1 on success, 0 if the file couldn't be opened. */
int pharmacy_save_to_csv(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;

    fprintf(fp, "pharmacy_name,address,eta_minutes,medicine_name,price,stock\n");
    for (int p = 0; p < pharmacyCount; p++) {
        for (int m = 0; m < pharmacies[p].medCount; m++) {
            fprintf(fp, "%s,%s,%.1f,%s,%.2f,%d\n",
                    pharmacies[p].name,
                    pharmacies[p].address,
                    pharmacies[p].etaMinutes,
                    pharmacies[p].meds[m].name,
                    pharmacies[p].meds[m].price,
                    pharmacies[p].meds[m].stock);
        }
    }

    fclose(fp);
    return 1;
}