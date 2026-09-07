/* ============================================================
   prescription.h
   ------------------------------------------------------------
   Module owner: [Member B]

   Responsible for getting the user's prescription into the
   program, either by reading a text file (one medicine name
   per line) or by manual keyboard entry.
   ============================================================ */

#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "common.h"

/* Prompts the user to choose an upload method (file or manual
   entry), reads the medicine names, and stores them into items[].
   Returns the number of medicine names captured (0 if none). */
int prescription_upload(char items[][MAX_NAME_LEN]);

#endif /* PRESCRIPTION_H */
