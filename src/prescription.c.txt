/* ============================================================
   prescription.c
   Module owner: [Member B]

   Implements prescription capture: either loading medicine
   names from a text file the user points to, or typing them in
   directly. Falls back to manual entry if the given file can't
   be opened.
   ============================================================ */

#include <stdio.h>
#include <string.h>

#include "../include/prescription.h"
#include "../include/utils.h"

/* Internal helper: manual keyboard entry of medicine names.
   Shared by the "manual entry" menu choice and the fallback
   path when a given file can't be opened. */
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
            printf("\nCould not open file \"%s\". Falling back to manual entry.\n", path);
            return manualEntry(items);
        }

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
