/* ============================================================
   utils.h
   ------------------------------------------------------------
   Small shared helpers (string trimming, case-insensitive
   comparison support, timing, console output) used by more
   than one module. Kept separate so no single module "owns"
   basic plumbing that everyone needs.
   ============================================================ */

#ifndef UTILS_H
#define UTILS_H

/* Removes trailing newline/carriage-return/whitespace and
   leading whitespace from a string, in place. */
void util_trim(char *s);

/* Copies src into dst, lower-casing every character.
   Caller must ensure dst is large enough (same size as src). */
void util_to_lower(const char *src, char *dst);

/* Sleeps for the given number of tenths of a second.
   Used to pace the drone animation so it's readable. */
void util_sleep_tenths(int tenths);

/* Prints a horizontal divider line, used to separate sections
   of console output consistently across modules. */
void util_print_divider(void);

#endif /* UTILS_H */
