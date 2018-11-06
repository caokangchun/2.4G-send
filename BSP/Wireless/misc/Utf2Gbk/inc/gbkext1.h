#ifndef __GBKEXT1_H
#define __GBKEXT1_H

#include "Encoding.h"
#include "EncodeDataExtract.h"

#define gbkext1_2uni_page81(i)      ExtractBit16(gbkext1_2uni_page81_Addr+(i)*2)

int gbkext1_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);

#endif 

