/* ============================================================
   common.h
   ------------------------------------------------------------
   Shared constants and data structures used across every
   module (pharmacy, prescription, order/drone, main).
   Keeping these in one header means every module agrees on the
   same definitions -- this is the "contract" between modules.
   ============================================================ */

#ifndef COMMON_H
#define COMMON_H

#define MAX_PHARMACIES         6
#define MAX_MEDS_PER_PHARMACY  8
#define MAX_NAME_LEN           64
#define MAX_ADDR_LEN           96
#define MAX_PRESCRIPTION_ITEMS 10
#define MAX_MATCHES            MAX_PHARMACIES

/* A single medicine sold at a pharmacy, with its price and stock level. */
typedef struct {
    char  name[MAX_NAME_LEN];
    float price;
    int   stock;
} Medication;

/* A pharmacy: identity, location, drone flight time, and its medicine list. */
typedef struct {
    char       name[MAX_NAME_LEN];
    char       address[MAX_ADDR_LEN];
    double     etaMinutes;      /* simulated drone flight time from this pharmacy */
    Medication meds[MAX_MEDS_PER_PHARMACY];
    int        medCount;
} Pharmacy;

/* One line item in a confirmed order: which medicine, from which
   pharmacy (by index into the pharmacy array), and at what price. */
typedef struct {
    char  medicineName[MAX_NAME_LEN];
    int   pharmacyIndex;
    float price;
} OrderItem;

#endif /* COMMON_H */
