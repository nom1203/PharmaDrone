/* ============================================================
   utils.c
   Implementation of shared string/timing helpers.
   ============================================================ */

#define _DEFAULT_SOURCE  /* needed for usleep() under -std=c11 */

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>

#include "../include/utils.h"

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

void util_to_lower(const char *src, char *dst) {
    int i = 0;
    for (; src[i]; i++) dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

void util_sleep_tenths(int tenths) {
    usleep(tenths * 100000);
}

void util_print_divider(void) {
    printf("------------------------------------------------------------\n");
}
