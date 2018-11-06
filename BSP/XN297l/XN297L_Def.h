
#ifndef __XN267L_H__
#define __XN267L_H__

/* RF COMMAND macro define */
#define		CMD_R_REGISTER			0x00    // SPI read RF data
#define		CMD_W_REGISTER			0x20    // SPI write RF data
#define		CMD_R_RX_PAYLOAD		0x61    // Read RX Payload
#define		CMD_W_TX_PAYLOAD		0xA0    // Write TX Payload
#define		CMD_FLUSH_TX			0xE1    // Flush RX FIFO
#define		CMD_FLUSH_RX			0xE2    // Flush TX FIFO
#define		CMD_REUSE_TX_PL		    0xE3    // Reuse TX Payload
#define		CMD_ACTIVATE			0x50    // ACTIVATE
#define		CMD_DEACTIVATE			0x50    // DEACTIVATE
#define		CMD_R_RX_PL_WID		    0x60    // Read width of RX data 
#define		CMD_W_ACK_PAYLOAD		0xA8    // Data with ACK
#define		CMD_W_TX_PAYLOAD_NOACK  0xB0    // TX Payload no ACK Request
#define		CMD_CE_FSPI_ON	        0xFD    // CE HIGH
#define		CMD_CE_FSPI_OFF	        0xFC    // CE LOW
#define		CMD_RST_FSPI	        0x53    // RESET
#define		CMD_NOP				    0xFF    // No Operation
             
/* RF CONTROL register define */
#define     REG_CONFIG              0x00
#define     _EN_PM                  0x80
#define     _MASK_RX_DR             0x40
#define     _MASK_TX_DS             0x20
#define     _MASK_MAX_RT            0x10
#define     _EN_CRC                 0x08
#define     _CRC_2B                 0x04
#define     _PWR_UP                 0x02
#define     _PRIM_RX                0x01

/* Enable/Disable Auto ACK */
#define     REG_EN_AA               0x01
#define     _ENAA_P5                0x20
#define     _ENAA_P4                0x10
#define     _ENAA_P3                0x08
#define     _ENAA_P2                0x04
#define     _ENAA_P1                0x02
#define     _ENAA_P0                0x01

/* Enable/Disable RX pipe number */
#define     REG_EN_RXADDR		    0x02
#define     _ENRX_P5                0x20
#define     _ENRX_P4                0x10
#define     _ENRX_P3                0x08
#define     _ENRX_P2                0x04
#define     _ENRX_P1                0x02
#define     _ENRX_P0                0x01

/* Address Width setup */
#define     REG_SETUP_AW		    0x03
#define     _SETUP_AW_3B            0x01
#define     _SETUP_AW_4B            0x02
#define     _SETUP_AW_5B            0x03

/* Retransmit count and delay setting */
#define     REG_SETUP_RETR          0x04
#define     _ARD_250us              0x00
#define     _ARD_500us              0x10
#define     _ARD_750us              0x20
#define     _ARD_1000us             0x30
#define     _ARD_1250us             0x40
#define     _ARD_1500us             0x50
#define     _ARD_1750us             0x60
#define     _ARD_2000us             0x70
#define     _ARD_2250us             0x80
#define     _ARD_2500us             0x90
#define     _ARD_2750us             0xA0
#define     _ARD_3000us             0xB0
#define     _ARD_3250us             0xC0
#define     _ARD_3500us             0xD0
#define     _ARD_3750us             0xE0
#define     _ARD_4000us             0xF0
#define     _ARC_0                  0x00
#define     _ARC_1                  0x01
#define     _ARC_2                  0x02
#define     _ARC_3                  0x03
#define     _ARC_4                  0x04
#define     _ARC_5                  0x05
#define     _ARC_6                  0x06
#define     _ARC_7                  0x07
#define     _ARC_8                  0x08
#define     _ARC_9                  0x09
#define     _ARC_10                 0x0A
#define     _ARC_11                 0x0B
#define     _ARC_12                 0x0C
#define     _ARC_13                 0x0D
#define     _ARC_14                 0x0E
#define     _ARC_15                 0x0F

/* Channel number is from 0 (2400) to 127 (2527) */
#define     REG_RF_CH               0x05

/* TX Power and Rate setting */
#define     REG_RF_SETUP            0x06
#define     _RF_DR_2M               0x40
#define     _RF_DR_1M               0x00
#define     _RF_DR_250K             0xC0
#define     _RF_DR_125K             0x80
#define     _PA_GC_L0               0x00
#define     _PA_GC_L1               0x08
#define     _PA_GC_L2               0x10
#define     _PA_GC_L3               0x18
#define     _PA_GC_L4               0x20
#define     _PA_GC_L5               0x28
#define     _PA_GC_L6               0x30
#define     _PA_GC_L7               0x38
#define     _RF_PA_PWR_L0           0x00
#define     _RF_PA_PWR_L1           0x01
#define     _RF_PA_PWR_L2           0x02
#define     _RF_PA_PWR_L3           0x03
#define     _RF_PA_PWR_L4           0x04
#define     _RF_PA_PWR_L5           0x05
#define     _RF_PA_PWR_L6           0x06
#define     _RF_PA_PWR_L7           0x07

/* Interrupt status */
#define     REG_STATUS              0x07
#define     _RX_DR                  0x40
#define     _TX_DS                  0x20
#define     _MAX_RT                 0x10
#define     _RX_P_NO_5              0x0C
#define     _RX_P_NO_4              0x0A
#define     _RX_P_NO_3              0x08
#define     _RX_P_NO_2              0x06
#define     _RX_P_NO_1              0x04
#define     _RX_P_NO_0              0x02
#define     _TX_FULL_S              0x01

/* AA Retransmition Setting */
#define     REG_OBSERVE_TX          0x08
/* Value from 0 to 15, clear by changing channel */
#define     _PLOS_CNT               0xF0
/* Value from 0 to 15, clear by sending packet successfully */
#define     _ARC_CNT                0xF0

#define     REG_DATAOUT             0x09

/* TX and RX Address setting */
#define     REG_RX_ADDR_P0          0x0A
#define	    REG_RX_ADDR_P1		    0x0B
#define	    REG_RX_ADDR_P2		    0x0C
#define	    REG_RX_ADDR_P3		    0x0D
#define	    REG_RX_ADDR_P4		    0x0E
#define	    REG_RX_ADDR_P5		    0x0F
#define	    REG_TX_ADDR             0x10

/* RX width, vaule from 1 to 32/64 */
#define	    REG_RX_PW_P0		    0x11
#define	    REG_RX_PW_P1		    0x12
#define	    REG_RX_PW_P2		    0x13
#define	    REG_RX_PW_P3		    0x14
#define	    REG_RX_PW_P4		    0x15
#define	    REG_RX_PW_P5		    0x16

/* FIFO Status */
#define     REG_FIFO_STATUS         0x17
#define     _TX_REUSE               0x40
#define     _TX_FULL                0x20
#define     _TX_EMPTY               0x10
#define     _RX_FULL                0x02
#define     _RX_EMPTY               0x01

#define     REG_DEMOD_CAL           0x19
#define     REG_RF_CAL2			    0x1A
#define     REG_DEM_CAL2		    0x1B

/* Enable/Disable Dynamic Paylod Per Pipe Setting */
#define	    REG_DYNPD			    0x1C
#define     _DPL_P5                 0x20
#define     _DPL_P4                 0x10
#define     _DPL_P3                 0x08
#define     _DPL_P2                 0x04
#define     _DPL_P1                 0x02
#define     _DPL_P0                 0x01

/* Enable/Disable Dynamic Payload , ACK Payload, Dynamic ACK function */
#define     REG_FEATURE             0x1D
#define     _CE_SEL                 0x20
#define     _DATA_LEN_SEL_32B       0x00
#define     _DATA_LEN_SEL_64B       0x18
#define     _EN_DPL                 0x04
#define     _EN_ACK_PAY             0x02
#define     _EN_DYN_ACK             0x01

#define     REG_RF_CAL				0x1E
#define     REG_BB_CAL				0x1F


#endif /* __XN267L_H__ */


