#ifndef __UTF8_H
#define __UTF8_H

#include "Encoding.h"

int utf8_wctomb (unsigned char *r, ucs4_t wc, int n);
int utf8_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);


#endif

