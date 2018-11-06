#ifndef __CP936_H
#define __CP936_H

#include "ces_gbk.h"

int cp936_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);
int cp936_wctomb (unsigned char *r, ucs4_t wc, int n);

#endif

