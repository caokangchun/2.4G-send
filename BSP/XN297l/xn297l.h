
#ifndef __BK2425_H__
#define __BK2425_H__

#include "XN297L_Def.h"

#define XN297L_CS_0				SEL_LOW
#define XN297L_CS_1				SEL_HIGH

#define XN297L_PKT				IRQ
#define XN297L_CE				//RF_SDN
#define DUMMY_BYTE				0x00

#define RF_250KBPS
#ifdef RF_250KBPS
#define _RF_RATE				_RF_DR_250K
#define RF_FIFO_MAX				16
#else
#define _RF_RATE				_RF_DR_1M
#define RF_FIFO_MAX				32
#endif

#define RF13dBm                 0x3F // 13dBm
#define RF10dBm                 0X0F // 10dBm
#define RF8dBm                  0x15 // 8dbm
#define RF7dBm                  0x07 // 7dbm
#define RF5dBm                  0x2c // 5dbm
#define RF4dBm                  0x06 // 4dbm
#define RF2dBm                  0x05 // 2dbm
#define RF0dBm                  0X0B // 0dBm
#define RF_3dBm                 0x04 // -3dBm
#define RF_6dBm                 0x0A // -6dBm
#define RF_10dBm                0x02 // -10dBm
#define RF_18dBm                0x01 // -18dBm
#define RF_30dBm                0x00 // -30dBm

#define _RF_POWER				RF10dBm

#define RF_AUTO_ACK
#define RF_ACK_PLD
#define RF_CE_PKT_SOFT

#ifdef  RF_CE_PKT_SOFT
#define RF_CE_SET_LOW           xn297l_set_ce_0();
#define RF_CE_SET_HIGH          xn297l_set_ce_1();
#else
#define RF_CE_SET_LOW           XN297L_CE = 0;
#define RF_CE_SET_HIGH          XN297L_CE = 1;
#endif

#define xn297l_spi_send(a)		sRF_SendByte(a)
#define xn297l_spi_recv(a)		sRF_SendByte(a)

#if 0
#define mcu_goto_idle()			__WFI();//__wait_for_interrupt();
#else
#define mcu_goto_idle()
#endif

#define MAC0                    0x5F
#define MAC1                    0x6E
#define MAC2                    0x7D
#define MAC3                    0x8C

extern const uint8_t rf_def_para[5];

typedef enum _RF_DIR
{
	RF_TRANSFER = 0,
	RF_RECEIVER
}RF_DIR;

typedef struct _RF_CONTROL
{
	uint8_t ch_no;
	uint8_t ch_index;
	uint8_t pipe_index;

	uint8_t mac[4];
	uint8_t rf_power;
}RF_CONTROL;

extern RF_CONTROL xn297l;
extern volatile uint8_t delay_tick;

BOOL xn297l_init(void);
void xn297l_goto_sleep(void);
void xn297l_goto_wakeup(void);
void xn297l_goto_pd(void);
void xn297l_set_channel(uint8_t rf_ch);
void xn297l_set_mac(uint8_t *mac);
void xn297l_read_1byte(uint8_t reg_addr, uint8_t *reg_dat);

BOOL xn297l_transceive (
	RF_DIR rf_dir, uint8_t pwr_ctrl, uint8_t t_out,
	uint8_t *tx_src, uint8_t tx_len,  
	uint8_t *rx_dst, uint8_t dst_len, uint8_t *rx_len
);

void RF_Carrier(uint8_t ucChannel_Set);

#endif /* __BK2425_H__ */


