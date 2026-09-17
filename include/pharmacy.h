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

/* Loads the pharmacy list from a CSV file, replacing whatever is
   currently in memory. Expected format (one line per medicine,
   with a header row that gets skipped):

       pharmacy_name,address,eta_minutes,medicine_name,price,stock

   Rows sharing the same pharmacy_name are grouped into a single
   Pharmacy entry. Returns 1 on success (file opened and at least
   one pharmacy loaded), or 0 if the file couldn't be opened or no
   valid rows were found -- in which case the in-memory list is
   left empty and the caller should fall back to pharmacy_init(). */
int pharmacy_load_from_csv(const char *filename);

/* Writes the current in-memory pharmacy list out to a CSV file in
   the same format read by pharmacy_load_from_csv(), one row per
   medicine (a pharmacy with no medicines is not written at all).
   Returns 1 on success, 0 if the file couldn't be opened for
   writing. */
int pharmacy_save_to_csv(const char *filename);

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