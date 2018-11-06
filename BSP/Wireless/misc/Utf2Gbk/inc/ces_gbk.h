#ifndef __CES_GBK_H
#define __CES_GBK_H

#include "ascii.h"
#include "gbk.h"

int ces_gbk_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);
int ces_gbk_wctomb (unsigned char *r, ucs4_t wc, int n);

#endif

