#ifndef __GBKEXT_INV_H
#define __GBKEXT_INV_H

#include "Encoding.h"
#include "EncodeDataExtract.h"

#define gbkext_inv_uni2indx_page02(i)   ExtractSummary16(gbkext_inv_uni2indx_page02_Addr+(i)*4)
#define gbkext_inv_uni2indx_page20(i)   ExtractSummary16(gbkext_inv_uni2indx_page20_Addr+(i)*4)
#define gbkext_inv_uni2indx_page25(i)   ExtractSummary16(gbkext_inv_uni2indx_page25_Addr+(i)*4)
#define gbkext_inv_uni2indx_page30(i)   ExtractSummary16(gbkext_inv_uni2indx_page30_Addr+(i)*4)
#define gbkext_inv_uni2indx_page32(i)   ExtractSummary16(gbkext_inv_uni2indx_page32_Addr+(i)*4)
#define gbkext_inv_uni2indx_page4e(i)   ExtractSummary16(gbkext_inv_uni2indx_page4e_Addr+(i)*4)
#define gbkext_inv_uni2indx_pagef9(i)   ExtractSummary16(gbkext_inv_uni2indx_pagef9_Addr+(i)*4)
#define gbkext_inv_uni2indx_pagefe(i)   ExtractSummary16(gbkext_inv_uni2indx_pagefe_Addr+(i)*4)
#define gbkext_inv_2charset(i)          ExtractBit16(gbkext_inv_2charset_Addr+(i)*2)

int gbkext_inv_wctomb (unsigned char *r, ucs4_t wc, int n);


#endif

