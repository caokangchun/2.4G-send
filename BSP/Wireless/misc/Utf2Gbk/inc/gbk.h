#ifndef __GBK_H
#define __GBK_H


#include "gbkext1.h"
#include "gbkext2.h"
#include "gbkext_inv.h"
#include "cp936ext.h"
#include "gb2312.h"

int gbk_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);
int gbk_wctomb (unsigned char *r, ucs4_t wc, int n);

#endif

