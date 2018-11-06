#ifndef __GBKEXT2_H
#define __GBKEXT2_H

#include "Encoding.h"
#include "EncodeDataExtract.h"

#define gbkext2_2uni_pagea8(i)  ExtractBit16(gbkext2_2uni_pagea8_Addr+(i)*2)

int gbkext2_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);

#endif

