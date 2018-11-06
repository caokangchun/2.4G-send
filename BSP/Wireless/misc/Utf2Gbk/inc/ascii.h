#ifndef __ASCII_H
#define __ASCII_H

#include "Encoding.h"

int ascii_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);
int ascii_wctomb (unsigned char *r, ucs4_t wc, int n);

#endif

