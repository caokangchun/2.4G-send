#ifndef __GB2312_H
#define __GB2312_h

#include "Encoding.h"
#include "EncodeDataExtract.h"

#define gb2312_2uni_page21(i)       ExtractBit16(gb2312_2uni_page21_Addr+(i)*2)
#define gb2312_2uni_page30(i)       ExtractBit16(gb2312_2uni_page30_Addr+(i)*2)
#define gb2312_uni2indx_page00(i)   ExtractSummary16(gb2312_uni2indx_page00_Addr+(i)*4)
#define gb2312_uni2indx_page20(i)   ExtractSummary16(gb2312_uni2indx_page20_Addr+(i)*4)
#define gb2312_uni2indx_page30(i)   ExtractSummary16(gb2312_uni2indx_page30_Addr+(i)*4)
#define gb2312_uni2indx_page4e(i)   ExtractSummary16(gb2312_uni2indx_page4e_Addr+(i)*4)
#define gb2312_uni2indx_page9e(i)   ExtractSummary16(gb2312_uni2indx_page9e_Addr+(i)*4)
#define gb2312_uni2indx_pageff(i)   ExtractSummary16(gb2312_uni2indx_pageff_Addr+(i)*4)
#define gb2312_2charset(i)          ExtractBit16(gb2312_2charset_Addr+(i)*2)

int gb2312_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);
int gb2312_wctomb (unsigned char *r, ucs4_t wc, int n);

#endif 

