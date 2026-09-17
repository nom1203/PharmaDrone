/* ============================================================
   server.c
   Tiny HTTP layer that exposes the pharmacy module to the
   browser. Keeps all C logic in the existing modules.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "civetweb.h"
#include "../include/pharmacy.h"
#include "../include/utils.h"

/* Escape a string so it can be embedded in JSON. */
static void json_escape(const char *src, char *dst, size_t dstsz) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j + 2 < dstsz; i++) {
        char c = src[i];
        if (c == '"' || c == '\\') { dst[j++] = '\\'; dst[j++] = c; }
        else if (c == '\n')        { dst[j++] = '\\'; dst[j++] = 'n'; }
        else                       { dst[j++] = c; }
    }
    dst[j] = '\0';
}

/* Read one query parameter (?key=value) into out. */
static int get_query_param(struct mg_connection *c, const char *key,
                           char *out, size_t outsz) {
    const char *qs = mg_get_request_info(c)->query_string;
    if (!qs) return 0;
    char pattern[64];
    snprintf(pattern, sizeof(pattern), "%s=", key);
    const char *p = strstr(qs, pattern);
    if (!p) return 0;
    p += strlen(pattern);
    const char *end = strchr(p, '&');
    size_t len = end ? (size_t)(end - p) : strlen(p);
    if (len >= outsz) len = outsz - 1;
    memcpy(out, p, len);
    out[len] = '\0';
    mg_url_decode(out, (int)len, out, (int)outsz, 0);
    return 1;
}

/* GET /api/pharmacies -> { "pharmacies": [ ... ] } */
static int h_pharmacies(struct mg_connection *c, void *ud) {
    (void)ud;
    mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                 "Access-Control-Allow-Origin: *\r\n\r\n");
    mg_printf(c, "{\"pharmacies\":[");
    int pc = pharmacy_get_count();
    for (int p = 0; p < pc; p++) {
        const Pharmacy *ph = pharmacy_get(p);
        char name[256], addr[256];
        json_escape(ph->name, name, sizeof(name));
        json_escape(ph->address, addr, sizeof(addr));
        mg_printf(c, "%s{\"index\":%d,\"name\":\"%s\",\"address\":\"%s\","
                     "\"eta\":%.1f,\"meds\":[",
                  p ? "," : "", p, name, addr, ph->etaMinutes);
        for (int m = 0; m < ph->medCount; m++) {
            char mn[256];
            json_escape(ph->meds[m].name, mn, sizeof(mn));
            mg_printf(c, "%s{\"name\":\"%s\",\"price\":%.2f,\"stock\":%d}",
                      m ? "," : "", mn, ph->meds[m].price, ph->meds[m].stock);
        }
        mg_printf(c, "]}");
    }
    mg_printf(c, "]}");
    return 200;
}

/* GET /api/match?med=Paracetamol -> { "matches": [ ... ] } */
static int h_match(struct mg_connection *c, void *ud) {
    (void)ud;
    char med[MAX_NAME_LEN];
    if (!get_query_param(c, "med", med, sizeof(med))) {
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\n\r\n"
                     "{\"error\":\"missing med\"}");
        return 400;
    }

    int idx[MAX_MATCHES];
    int n = pharmacy_find_matches(med, idx);

    mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                 "Access-Control-Allow-Origin: *\r\n\r\n");
    mg_printf(c, "{\"medicine\":\"%s\",\"matches\":[", med);
    for (int i = 0; i < n; i++) {
        const Pharmacy *ph = pharmacy_get(idx[i]);
        Medication m;
        pharmacy_get_medication(idx[i], med, &m);
        char name[256], addr[256], mn[256];
        json_escape(ph->name, name, sizeof(name));
        json_escape(ph->address, addr, sizeof(addr));
        json_escape(m.name, mn, sizeof(mn));
        mg_printf(c, "%s{\"index\":%d,\"name\":\"%s\",\"address\":\"%s\","
                     "\"eta\":%.1f,\"med\":\"%s\",\"price\":%.2f,\"stock\":%d}",
                  i ? "," : "", idx[i], name, addr, ph->etaMinutes,
                  mn, m.price, m.stock);
    }
    mg_printf(c, "]}");
    return 200;
}

int main(void) {
    /* Same load logic as main.c. */
    if (!pharmacy_load_from_csv("data/pharmacies.csv")) {
        printf("(No data/pharmacies.csv found -- using built-in sample data.)\n");
        pharmacy_init();
    }

    const char *options[] = {
        "listening_ports", "8080",
        "document_root",   "web",   /* serves index.html, style.css, app.js */
        NULL
    };

    struct mg_callbacks cb;
    memset(&cb, 0, sizeof(cb));
    struct mg_context *ctx = mg_start(&cb, NULL, options);
    if (!ctx) { fprintf(stderr, "mg_start failed\n"); return 1; }

    mg_set_request_handler(ctx, "/api/pharmacies", h_pharmacies, NULL);
    mg_set_request_handler(ctx, "/api/match",      h_match,      NULL);

    printf("PharmaDrone server running at http://localhost:8080\n");
    printf("Press Enter to stop.\n");
    getchar();

    mg_stop(ctx);
    return 0;
}