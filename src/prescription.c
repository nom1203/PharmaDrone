/* ============================================================
   prescription.c
   Module owner: [Member B]
 
   Gets the prescription into the program -- either by reading
   a text file (one medicine per line) or by manual entry.
   Falls back to manual entry if the file can't be opened.
   ============================================================ */

#include <stdio.h>
#include <string.h>

#include "../include/prescription.h"
#include "../include/utils.h"
 
/* Lets the user type medicine names in one at a time until they
   type 'done' or hit the item limit. Used both for manual entry
   and as a fallback if a prescription file can't be opened. */
static int manualEntry(char items[][MAX_NAME_LEN]) {
    int count = 0;

    printf("\nType each medication name, then press Enter.\n");
    printf("Type 'done' when finished (max %d items).\n\n", MAX_PRESCRIPTION_ITEMS);

    while (count < MAX_PRESCRIPTION_ITEMS) {
        char line[MAX_NAME_LEN];
        printf("  Medicine %d (or 'done'): ", count + 1);
        if (!fgets(line, sizeof(line), stdin)) break;
        util_trim(line);

        if (strlen(line) == 0) continue;
        if (strcmp(line, "done") == 0 || strcmp(line, "Done") == 0) break;

        strncpy(items[count], line, MAX_NAME_LEN - 1);
        items[count][MAX_NAME_LEN - 1] = '\0';
        count++;
    }

    printf("\nPrescription captured: %d medication(s).\n\n", count);
    return count;
}
 
/* Asks the user how they want to give their prescription, then
   reads it in and stores the medicine names into items[].
   Returns how many medicines were captured. */
int prescription_upload(char items[][MAX_NAME_LEN]) {
    char choice[8];

    util_print_divider();
    printf("UPLOAD PRESCRIPTION\n");
    util_print_divider();
    printf("How would you like to upload your prescription?\n");
    printf("  1. Load from a text file (one medicine per line)\n");
    printf("  2. Type medicines in manually\n");
    printf("Choice: ");

    if (!fgets(choice, sizeof(choice), stdin)) return 0;
    util_trim(choice);

    if (strcmp(choice, "1") == 0) {
        char path[256];
        printf("Enter the path to your prescription file: ");
        if (!fgets(path, sizeof(path), stdin)) return 0;
        util_trim(path);

        FILE *fp = fopen(path, "r");
        if (!fp) {
            /* Bad path or unreadable file -- don't dead-end the
               user, just drop them into manual entry instead. */
            printf("\nCould not open file \"%s\". Falling back to manual entry.\n", path);
            return manualEntry(items);
        }
 
        /* Read one medicine name per line until the file ends or
           we hit the max number of items we can store. */
        int count = 0;
        char line[MAX_NAME_LEN];
        while (fgets(line, sizeof(line), fp) && count < MAX_PRESCRIPTION_ITEMS) {
            util_trim(line);
            if (strlen(line) > 0) {
                strncpy(items[count], line, MAX_NAME_LEN - 1);
                items[count][MAX_NAME_LEN - 1] = '\0';
                count++;
            }
        }
        fclose(fp);
        printf("\nLoaded %d medication(s) from file.\n\n", count);
        return count;
    }

    /* Any choice other than "1" (including "2") goes to manual entry. */
    return manualEntry(items);
}
