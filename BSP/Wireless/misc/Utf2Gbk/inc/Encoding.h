#ifndef _ENCODING_H
#define _ENCODING_H

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
  unsigned short indx; /* index into big table */
  unsigned short used; /* bitmask of used entries */
} Summary16;

/* Return code if invalid. (xxx_wctomb) */
#define RET_ILUNI      -1
/* Return code if output buffer is too small. (xxx_wctomb, xxx_reset) */
#define RET_TOOSMALL   -2

/* Return code if invalid input after a shift sequence of n bytes was read.
   (xxx_mbtowc) */
#define RET_SHIFT_ILSEQ(n)  (-1-2*(n))
/* Return code if invalid. (xxx_mbtowc) */
#define RET_ILSEQ           RET_SHIFT_ILSEQ(0)
/* Return code if only a shift sequence of n bytes was read. (xxx_mbtowc) */
#define RET_TOOFEW(n)       (-2-2*(n))
/* Retrieve the n from the encoded RET_... value. */
#define DECODE_SHIFT_ILSEQ(r)  ((unsigned int)(RET_SHIFT_ILSEQ(0) - (r)) / 2)
#define DECODE_TOOFEW(r)       ((unsigned int)(RET_TOOFEW(0) - (r)) / 2)

/* Our own notion of wide character, as UCS-4, according to ISO-10646-1. */
typedef unsigned int ucs4_t;

/**********************CP936************************/
#define gb2312_2uni_page21_Addr	        0
#define gb2312_2uni_page30_Addr	        1662
#define cp936ext_2uni_pagea6_Addr	    15198
#define cp936ext_2uni_pagea8_Addr	    15242
#define gbkext1_2uni_page81_Addr	    15254
#define gbkext2_2uni_pagea8_Addr	    27414
#define gb2312_2charset_Addr                 43958
#define gb2312_uni2indx_page00_Addr          73738
#define gb2312_uni2indx_page20_Addr          74018
#define gb2312_uni2indx_page30_Addr          74422
#define gb2312_uni2indx_page4e_Addr          74562
#define gb2312_uni2indx_page9e_Addr          79614
#define gb2312_uni2indx_pageff_Addr          79722
#define cp936ext_page01_Addr                 79782
#define cp936ext_page02_Addr                 79814
#define cp936ext_pagefe_Addr                 79862
#define gbkext_inv_2charset_Addr             79910
#define gbkext_inv_uni2indx_page02_Addr      108536
#define gbkext_inv_uni2indx_page20_Addr      108592
#define gbkext_inv_uni2indx_page25_Addr      108768
#define gbkext_inv_uni2indx_page30_Addr      108836
#define gbkext_inv_uni2indx_page32_Addr      108900
#define gbkext_inv_uni2indx_page4e_Addr      109020
#define gbkext_inv_uni2indx_pagef9_Addr      114248
#define gbkext_inv_uni2indx_pagefe_Addr      114324

#endif
