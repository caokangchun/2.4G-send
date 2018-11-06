/*
 * Copyright (C) 1999-2000 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
 * GBK/3 extensions
 */
#include "gbkext1.h"

int
gbkext1_mbtowc (ucs4_t *pwc, const unsigned char *s, int n)
{
  unsigned char c1 = s[0];
  if ((c1 >= 0x81 && c1 <= 0xa0)) {
    if (n >= 2) {
      unsigned char c2 = s[1];
      if ((c2 >= 0x40 && c2 < 0x7f) || (c2 >= 0x80 && c2 < 0xff)) {
        unsigned int i = 190 * (c1 - 0x81) + (c2 - (c2 >= 0x80 ? 0x41 : 0x40));
        unsigned short wc = 0xfffd;
        {
          if (i < 6080)
            wc = gbkext1_2uni_page81(i);
        }
        if (wc != 0xfffd) {
          *pwc = (ucs4_t) wc;
          return 2;
        }
      }
      return RET_ILSEQ;
    }
    return RET_TOOFEW(0);
  }
  return RET_ILSEQ;
}

