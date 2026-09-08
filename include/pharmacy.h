/* ============================================================
   pharmacy.h
   ------------------------------------------------------------
   Module owner: [Ajlaan]

   Responsible for the pharmacy "database" (in-memory for this
   version) and everything related to searching it: which
   pharmacies stock a given medicine, at what price/stock level,
   and printing the full catalog.
   ============================================================ */

#ifndef PHARMACY_H
#define PHARMACY_H

#include "common.h"

/* Populates the in-memory pharmacy list with sample data.
   Must be called once before any other pharmacy_* function. */
void pharmacy_init(void);

/* Returns how many pharmacies currently exist in the catalog. */
int pharmacy_get_count(void);

/* Returns a read-only pointer to the pharmacy at the given index,
   or NULL if the index is out of range. */
const Pharmacy *pharmacy_get(int index);

/* Searches every pharmacy for medName (case-insensitive) and
   fills matchIndices[] with the indices of pharmacies that have
   it in stock (stock > 0). Returns the number of matches found.
   matchIndices must have room for at least MAX_MATCHES entries. */
int pharmacy_find_matches(const char *medName, int matchIndices[]);

/* Looks up medName (case-insensitive) at the given pharmacy index.
   If found, copies its details into *out and returns 1.
   If not found, returns 0 and *out is left unchanged. */
int pharmacy_get_medication(int pharmacyIndex, const char *medName, Medication *out);

/* Prints every pharmacy and its full medicine list (including
   out-of-stock items) to stdout. */
void pharmacy_print_catalog(void);

#endif /* PHARMACY_H */
