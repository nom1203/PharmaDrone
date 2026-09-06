/* ============================================================
   order.h
   ------------------------------------------------------------
   Module owner: [Member C]

   Ties the prescription and pharmacy modules together: for
   each medicine on the prescription, lets the user pick a
   pharmacy, builds the final order, and runs the simulated
   drone pickup/delivery.
   ============================================================ */

#ifndef ORDER_H
#define ORDER_H

#include "common.h"

/* Runs the full interactive order flow: upload prescription,
   search + select a pharmacy per medicine, confirm, and dispatch
   the drone. This is the main entry point called from the menu. */
void order_run_flow(void);

/* Pure calculation, kept separate from I/O so it can be unit
   tested directly: sums the price of every item in orders[]. */
float order_calculate_total(const OrderItem orders[], int count);

/* Runs the console "drone animation": visits each distinct
   pharmacy used in the order, simulates pickup, simulates
   delivery, then prints an itemized receipt. */
void order_dispatch_drone(OrderItem orders[], int orderCount, float totalCost);

#endif /* ORDER_H */
