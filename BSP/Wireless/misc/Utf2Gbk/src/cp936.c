/*
 * Copyright (C) 2005 Free Software Foundation, Inc.
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
 * CP936
 */

/*
 * The IANA has CP936 as an alias of GBK. But GBK is an official Chinese
 * specification, whereas CP936 is de-facto maintained by Microsoft. And,
 * of course, Microsoft modified CP936 since 1999.
 *
 * The differences from GBK are:
 *
 * 1. A single character:
 *
 *    code   CP936.TXT
 *    0x80   0x20AC # EURO SIGN
 *
 * Some variants of CP936 (in JDK, Windows-2000, ICU) also add:
 *
 * 2. Private area mappings:
 *
 *              code                 Unicode
 *    0x{A1..A2}{40..7E,80..A0}  U+E4C6..U+E585
 *    0x{AA..AF,F8..FE}{A1..FE}  U+E000..U+E4C5
 *
 * We add them too because, although there are backward compatibility problems
 * when a character from a private area is moved to an official Unicode code
 * point, they are useful for some people in practice.
 */

#include "cp936.h"


int
cp936_mbtowc (ucs4_t *pwc, const unsigned char *s, int n)
{
  /* Try GBK first. */
  {
    int ret = ces_gbk_mbtowc(pwc,s,n);
    if (ret != RET_ILSEQ)
      return ret;
  }
  /* Then handle the additional mappings. */
  {
    unsigned char c = *s;
    if (c == 0x80) {
      *pwc = 0x20ac;
      return 1;
    }
//    /* User-defined characters */
//    if (c >= 0xa1 && c <= 0xa2) {
//      if (n < 2)
//        return RET_TOOFEW(0);
//      {
//        unsigned char c2 = s[1];
//        if ((c2 >= 0x40 && c2 < 0x7f) || (c2 >= 0x80 && c2 < 0xa1)) {
//          *pwc = 0xe4c6 + 96 * (c - 0xa1) + (c2 - (c2 >= 0x80 ? 0x41 : 0x40));
//          return 2;
//        }
//      }
//    } else if ((c >= 0xaa && c < 0xb0) || (c >= 0xf8 && c < 0xff)) {
//      if (n < 2)
//        return RET_TOOFEW(0);
//      {
//        unsigned char c2 = s[1];
//        if (c2 >= 0xa1 && c2 < 0xff) {
//          *pwc = 0xe000 + 94 * (c - (c >= 0xf8 ? 0xf2 : 0xaa)) + (c2 - 0xa1);
//          return 2;
//        }
//      }
//    }
    /*GBK中占用 Unicode 用户自定义区的字符*/
    if(c==0xA2)
    {
      if (n < 2)
        return RET_TOOFEW(0); 
      unsigned char c2 = s[1];
      
      if(c2==0xE3)
      {
        *pwc=0xE76C;
        return 2;
      }
    }
    else if(c==0xA6)
    {
      if (n < 2)
        return RET_TOOFEW(0); 
      unsigned char c2 = s[1];
      if(c2 >= 0xD9 && c2 < 0xE0){
          *pwc = 0xe78d +c2-0xd9 ;
          return 2;            
      }else if(c2 >= 0xec && c2 <0xee){
          *pwc = 0xe794 +c2-0xec ;
          return 2;            
      }
      else if(c2 ==0xf3) {
          *pwc = 0xe796 ;
          return 2;         
      }      
    }
    else if(c==0xA8)
    {
      if (n < 2)
        return RET_TOOFEW(0);   
      unsigned char c2 = s[1];
      if(c2 == 0xbc) {
          *pwc = 0xe7c7 ;
          return 2;         
      } else if(c2 == 0xbf) {
          *pwc = 0xe7c8;
          return 2;          
      }
    }
    else if(c==0xA9)
    {
      if (n < 2)
        return RET_TOOFEW(0);  
      unsigned char c2 = s[1];
      if(c2 >= 0x89 && c2 < 0x96) {
          *pwc = 0xe7e7 + c2 - 0x89;
          return 2;         
      }else if(c2 >= 0xf0 && c2 < 0xf5) {
          *pwc = 0xe801 + c2 - 0xf0;
          return 2;         
      }
    }
    else if(c==0xFE)
    {
      if (n < 2)
        return RET_TOOFEW(0);   
      unsigned char c2 = s[1];
      if(c2 >= 0x50 && c2 < 0x7f) {
          *pwc = 0xe815 + c2 - 0x50;
          return 2;          
      }else if(c2 >= 0x80 && c2 < 0xa1) {
          *pwc = 0xe844 + c2 - 0x80;
          return 2;          
      }
    }
      
  }
  return RET_ILSEQ;
}

int
cp936_wctomb (unsigned char *r, ucs4_t wc, int n)
{
  /* Try GBK first. */
  {
    int ret = ces_gbk_wctomb(r,wc,n);
    if (ret != RET_ILUNI)
      return ret;
  }
//  /* Then handle the additional mappings. */
//  if (wc >= 0xe000 && wc < 0xe586) {
//    /* User-defined characters */
//    if (n < 2)
//      return RET_TOOFEW(0);
//    if (wc < 0xe4c6) {
//      unsigned int i = wc - 0xe000;
//      unsigned int c1 = i / 94;
//      unsigned int c2 = i % 94;
//      r[0] = c1 + (c1 < 6 ? 0xaa : 0xf2);
//      r[1] = c2 + 0xa1;
//      return 2;
//    } else {
//      unsigned int i = wc - 0xe4c6;
//      unsigned int c1 = i / 96;
//      unsigned int c2 = i % 96;
//      r[0] = c1 + 0xa1;
//      r[1] = c2 + (c2 < 0x3f ? 0x40 : 0x41);
//      return 2;
//    }
//  } else 
    if (wc == 0x20ac) {
    r[0] = 0x80;
    return 1;
  }
    unsigned int i;  
  if(wc == 0xE76C)
  {
    r[0] = 0xa2;
    r[1] = 0xe3;
    return 2;    
  } else if(wc >= 0xe78d && wc < 0xe794)  {
    i =0xa6d9 +( wc - 0xe78d);
    r[0] = (i>>8);
    r[1] = i&0xff;
    return 2;   
  } else if(wc >= 0xe794 && wc < 0xe796)  {
    i =0xa6ec +( wc - 0xe794);
    r[0] = (i>>8);
    r[1] = i&0xff;
    return 2;       
  } else if(wc == 0xe796)   {
    i =0xa6f3;
    r[0] = (i>>8);
    r[1] = i&0xff;
    return 2;    
  } else if(wc == 0xe7c7)   {
    i =0xa8bc;
    r[0] = (i>>8);
    r[1] = i&0xff;  
    return 2; 
  } else if(wc == 0xe7c8)   {
    i =0xa8bf;
    r[0] = (i>>8);
    r[1] = i&0xff;   
    return 2;
  } else if(wc >= 0xe7e7 && wc < 0xe7f4)   {
    i =0xa989 +(wc - 0xe7e7);
    r[0] = (i>>8);
    r[1] = i&0xff; 
    return 2;
  } else if(wc >= 0xe801 && wc < 0xe806)   {
    i =0xa9f0 + (wc - 0xe801);
    r[0] = (i>>8);
    r[1] = i&0xff; 
    return 2;
  } else if(wc >= 0xe815 && wc < 0xe844)   {
    i =0xfe50 + (wc - 0xe815);
    r[0] = (i>>8);
    r[1] = i&0xff; 
    return 2;
  } else if (wc >= 0xe844 && wc < 0xe865)  {
    i =0xfe80 + (wc - 0xe844);
    r[0] = (i>>8);
    r[1] = i&0xff;  
    return 2;
  }
  
  return RET_ILUNI;
}
