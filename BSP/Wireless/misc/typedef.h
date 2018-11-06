
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdint.h>

#if 1
//typedef signed char 	INT8;
typedef unsigned char 	uint8_t;
//typedef signed short 	INT16;
typedef unsigned short 	UINT16;
//typedef signed long		INT32;
typedef unsigned long	UINT32;
typedef unsigned char	BOOL;
#else
#define uint8_t   uint8_t
#define INT8    int8_t
#define UINT16  uint16_t
#define UINT32  uint32_t
#define BOOL    uint8_t
#endif /* 0 */
#define INT8    int8_t

//#ifndef FALSE
//#define FALSE   0
//#endif

//#ifndef TRUE
//#define TRUE    1
//#endif

//#ifndef NULL
//#define NULL	0
//#endif

#ifndef OFF
#define OFF				0
#endif

#ifndef ON
#define ON				1
#endif

#define BIT_MASK(a)			(1 << a)
#define BITS_SET(a, b)		(a |= b)
#define BIT_CLEAR(a, b)		(a &= ~ BIT_MASK(b))
#define BITS_CLEAR(a, b)	(a &= ~ b)

#define DAT_LEFT(a, b)		(a << b)
#define DAT_RIGHT(a, b)		(a >> b)

#define UNUSED(x)   		(void)x;

#endif /* end of __TYPEDEF_H__ */

