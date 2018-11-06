
#ifndef __RF_HAL_H__
#define __RF_HAL_H__

#include "xn297l.h"

#define STX					0x02
#define ETX					0x03
#define ACK_R				4
#define ACK_C				4
#define CH_INDEX_MAX		3
#define PIPE_INDEX_MAX		25

extern const uint8_t rf_ack[ACK_R][ACK_C];

BOOL rf_init(void);
void rf_set_pipe(uint8_t pipe_x);
uint8_t rf_set_ch(void);
void rf_set_mac(uint8_t *mac);
BOOL rf_transceiver (
	RF_DIR rf_dir, uint8_t t_out,
	uint8_t *tx_src, uint8_t tx_len,
	uint8_t *rx_dst, uint8_t dst_len, uint8_t *rx_len
);

#endif /* end of __RF_HAL_H__ */


