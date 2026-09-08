/* ============================================================
   utils.c
   Shared string/timing helpers used across modules.
   ============================================================ */

#define _DEFAULT_SOURCE  /* needed for usleep() under -std=c11 */

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>

#include "../include/utils.h"
 
/* Strips leading/trailing whitespace (and the newline fgets
   leaves at the end of a line) from a string, in place. */
void util_trim(char *s) {
    size_t len = strlen(s);
    while (len > 0 &&
           (s[len - 1] == '\n' || s[len - 1] == '\r' || isspace((unsigned char)s[len - 1]))) {
        s[--len] = '\0';
    }

    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, strlen(s + start) + 1);
}
 
/* Copies src into dst, lower-casing every character.
   Used to make string comparisons case-insensitive. */
void util_to_lower(const char *src, char *dst) {
    int i = 0;
    for (; src[i]; i++) dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

/* Sleeps for the given number of tenths of a second.
   Used to pace out the drone animation frames. */
void util_sleep_tenths(int tenths) {
    usleep(tenths * 100000);
}
 
/* Prints a horizontal divider line between sections of output. */
void util_print_divider(void) {
    printf("------------------------------------------------------------\n");
}
