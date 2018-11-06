
#ifndef __RF_CMD__
#define __RF_CMD__

#if 1//def _2D_SUP
#define HEAD_LEN	6
#define CMD_NO		6
#define LEN_H       1
#define LEN_L       2
#define PID_NO      3
#define HID_NO      4
#define LAN_NO      5
#else
#define LEN_NO      0
#define PID_NO      1
#define HID_NO      2
#define LAN_NO      3
#define HEAD_LEN	4
#define CMD_NO		3
#endif /* _2D_SUP */

#define LEN0		4
#define LEN1		6
#define LEN2		8
#define LEN3		9
#define LEN4		10
#define LEN5		11
#define LEN6		12

typedef enum _CMD_TYPE
{
	CMD_NONE = 0,

    CMD_LEN0_0, // language_en
    CMD_LEN0_1, // language_ge
    CMD_LEN0_2, // language_fr
    CMD_LEN0_3, // language_it
    CMD_LEN0_4, // language_fi
    CMD_LEN0_5, // language_es
    CMD_LEN0_6, // language_pt
    CMD_LEN0_7, // language_nw
    CMD_LEN0_8, // language_ru
    CMD_LEN0_9, // language_tk
	
	CMD_LEN1_0, // bat_volume

	CMD_LEN2_0, // one_key_pair
    CMD_LEN2_1, // user_id_set
    CMD_LEN2_2, // user_id_on
    CMD_LEN2_3, // user_id_off
    CMD_LEN2_4, // set_para_x

	CMD_LEN3_0, // buzzer_open1
	CMD_LEN3_1, // buzzer_open2
	CMD_LEN3_2, // software_ver
	CMD_LEN3_3, // factory_rst
    CMD_LEN3_4, // ticket_on
    CMD_LEN3_5, // ticket_off

	CMD_LEN4_0, // buzzer_close1
	CMD_LEN4_1, // buzzer_close2
	CMD_LEN4_2, // power_off
    CMD_LEN4_3, // sleep_tim_0x

	CMD_LEN5_0, // sleep_tim_set
	CMD_LEN5_1, // sleep_tim_dis
	CMD_LEN5_2, // normal_mode
	CMD_LEN5_3, // upload_data
	CMD_LEN5_4, // total_count
	CMD_LEN5_5, // clear_data
	CMD_LEN5_6, // hardware_ver

	CMD_LEN6_0, // store_mode
	CMD_LEN6_1, // unlost_mode
}CMD_TYPE;

#define CMD_LEN1	LEN1+HEAD_LEN
#define CMD_LEN2	LEN2+HEAD_LEN
#define CMD_LEN3	LEN3+HEAD_LEN
#define CMD_LEN4	LEN4+HEAD_LEN
#define CMD_LEN5	LEN5+HEAD_LEN

#define SET_BUF		3
typedef struct
{
	uint8_t ch;			// set ch
	uint8_t id;			// set id
	uint8_t host;//  : 2;	// host set
	uint8_t slave;// : 1;	// slave set
	uint8_t type;//  : 2;	// set type
	uint8_t len;//   : 3;	// data len
	uint8_t dat[SET_BUF];	// data buffer
}MENU_SET;

typedef enum
{
	SET_NONE = 0,
	SET_CH,
	SET_ID,
	SET_BUZZ,
	//SET_RX_ID,
}SET_TYPE;

#define ENTER   0x0D

extern const UINT16 sleep_tbl[8];
extern const char *SW_VERSION;

uint8_t rf_cmd_judge(uint8_t *cmd_dat, UINT16 cmd_len);
BOOL rf_host_cmd(uint8_t *cmd_dat);
uint8_t rf_lang_get(uint8_t lang);


#endif /* end of __RF_CMD__ */

