
#ifndef __CHECKOUT_H__
#define __CHECKOUT_H__

#include "typedef.h"

BOOL data_xor_check(const uint8_t *src, uint8_t len, uint8_t xor);
uint8_t calc_xor_value(const uint8_t *src, uint8_t len);
BOOL data_zero_check(const uint8_t *src, uint8_t len);
UINT16 calc_crc16_ccitt(uint8_t *pdat, uint8_t len);


#endif /* end of __CHECKOUT_H__ */

