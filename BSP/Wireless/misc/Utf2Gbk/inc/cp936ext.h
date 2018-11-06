#ifndef __CP936EXT_H
#define __CP936EXT_H

#include "Encoding.h"
#include "EncodeDataExtract.h"

#define cp936ext_2uni_pagea6(i)     ExtractBit16(cp936ext_2uni_pagea6_Addr+(i)*2)
#define cp936ext_2uni_pagea8(i)     ExtractBit16(cp936ext_2uni_pagea8_Addr+(i)*2)
#define cp936ext_page01(i)          ExtractBit16(cp936ext_page01_Addr+(i)*2)
#define cp936ext_page02(i)          ExtractBit16(cp936ext_page02_Addr+(i)*2)
#define cp936ext_pagefe(i)          ExtractBit16(cp936ext_pagefe_Addr+(i)*2)

int cp936ext_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);
int cp936ext_wctomb (unsigned char *r, ucs4_t wc, int n);

#endif

