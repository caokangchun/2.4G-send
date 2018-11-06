
#include "includes.h"

#define HW_VERSION      "HW_M3_2.4G_BT_Ver1.0"

const char *SW_VERSION = "BAR_SCANNER_M3_2.4G_BT_V1.0";

//#define _NO_STORE

#define code const
/* ----------------- rf cmd data ------------------------------------------------------------------------- */
code uint8_t one_key_pair[8]          = {0x25, 0x25, 0x45, 0x5A, 0x50, 0x61, 0x69, 0x72}; // %%EZPair

code uint8_t user_id_set[8]           = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2D, 0x49, 0x44}; // %%ALL-ID
code uint8_t user_id_on[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2D, 0x58, 0x53}; // %%ALL-XS
code uint8_t user_id_off[8]           = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2D, 0x59, 0x43}; // %%ALL-YC

code uint8_t set_para_x[7]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F};       // %%ALL*_x
code uint8_t set_para_0[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x30}; // %%ALL*_0
code uint8_t set_para_1[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x31}; // %%ALL*_1
code uint8_t set_para_2[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x32}; // %%ALL*_2
code uint8_t set_para_3[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x33}; // %%ALL*_3
code uint8_t set_para_4[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x34}; // %%ALL*_4
code uint8_t set_para_5[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x35}; // %%ALL*_5
code uint8_t set_para_6[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x36}; // %%ALL*_6
code uint8_t set_para_7[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x37}; // %%ALL*_7
code uint8_t set_para_8[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x38}; // %%ALL*_8
code uint8_t set_para_9[8]            = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x5F, 0x39}; // %%ALL*_9

code uint8_t sleep_tim_set[11]        = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x53, 0x45, 0x54}; // %%ALLTIMSET
code uint8_t sleep_tim_dis[11]        = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x2A, 0x54, 0x49, 0x44, 0x49, 0x53}; // %%ALL*TIDIS

code uint8_t sleep_tim_0x[9]          = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30};             // %%ALLTIM0x
code uint8_t sleep_tim_00[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x30};       // %%ALLTIM00
code uint8_t sleep_tim_01[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x31};       // %%ALLTIM01
code uint8_t sleep_tim_02[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x32};       // %%ALLTIM02
code uint8_t sleep_tim_03[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x33};       // %%ALLTIM03
code uint8_t sleep_tim_04[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x34};       // %%ALLTIM04
code uint8_t sleep_tim_05[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x35};       // %%ALLTIM05
code uint8_t sleep_tim_06[10]         = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x54, 0x49, 0x4D, 0x30, 0x36};       // %%ALLTIM06

code uint8_t normal_mode[11]          = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x50, 0x54, 0x2D, 0x53, 0x45, 0x54};       // %%ALLPT-SET
code uint8_t store_mode[12]           = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x4D, 0x45, 0x4D, 0x2D, 0x53, 0x45, 0x54}; // %%ALLMEM-SET
code uint8_t unlost_mode[12]          = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x41, 0x45, 0x4D, 0x2D, 0x53, 0x45, 0x54}; // %%ALLAEM-SET
code uint8_t upload_data[11]          = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x4D, 0x45, 0x4D, 0x2D, 0x53, 0x43};       // %%ALLAEM-SC
code uint8_t total_count[11]          = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x4D, 0x45, 0x4D, 0x2D, 0x5A, 0x53};       // %%ALLAEM-ZS
code uint8_t clear_data[11]           = {0x25, 0x25, 0x41, 0x4C, 0x4C, 0x4D, 0x45, 0x4D, 0x2D, 0x51, 0x43};       // %%ALLAEM-QC

code uint8_t language_en[4]           = {0x25, 0x25, 0x45, 0x4E}; // %%EN
code uint8_t language_ge[4]           = {0x25, 0x25, 0x47, 0x45}; // %%GE
code uint8_t language_fr[4]           = {0x25, 0x25, 0x46, 0x52}; // %%FR
code uint8_t language_it[4]           = {0x25, 0x25, 0x49, 0x54}; // %%IT
code uint8_t language_fi[4]           = {0x25, 0x25, 0x46, 0x49}; // %%FI
code uint8_t language_es[4]           = {0x25, 0x25, 0x45, 0x53}; // %%ES
code uint8_t language_pt[4]           = {0x25, 0x25, 0x50, 0x54}; // %%PT
code uint8_t language_nw[4]           = {0x25, 0x25, 0x4E, 0x57}; // %%NW
code uint8_t language_ru[4]           = {0x25, 0x25, 0x52, 0x55}; // %%RU
code uint8_t language_tk[4]           = {0x25, 0x25, 0x54, 0x4B}; // %%TK

code uint8_t buzzer_open1[9]          = {0x25, 0x25, 0x42, 0x5A, 0x4F, 0x70, 0x65, 0x6E, 0x31};       // %%BZOpen1
code uint8_t buzzer_close1[10]        = {0x25, 0x25, 0x42, 0x5A, 0x43, 0x6C, 0x6F, 0x73, 0x65, 0x31}; // %%BZClose1
code uint8_t buzzer_open2[9]          = {0x25, 0x25, 0x42, 0x5A, 0x4F, 0x70, 0x65, 0x6E, 0x32};       // %%BZOpen2
code uint8_t buzzer_close2[10]        = {0x25, 0x25, 0x42, 0x5A, 0x43, 0x6C, 0x6F, 0x73, 0x65, 0x32}; // %%BZClose2

code uint8_t bat_volume[6]            = {0x25, 0x25, 0x42, 0x61, 0x74, 0x74}; // %%Batt

code uint8_t power_off[10]            = {0x25, 0x25, 0x50, 0x4F, 0x57, 0x45, 0x52, 0x4F, 0x46, 0x46};       // %%POWEROFF
code uint8_t software_ver[9]          = {0x25, 0x25, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E};             // %%Version
code uint8_t hardware_ver[11]         = {0x25, 0x25, 0x48, 0x44, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E}; // %%HDVersion
code uint8_t factory_rst[9]           = {0x25, 0x25, 0x52, 0x65, 0x73, 0x74, 0x6F, 0x72, 0x65};             // %%Restore

code uint8_t ticket_off[9]            = {0x00, 0x52, 0x44, 0x39, 0x46, 0x46, 0x35, 0x30, 0x2E}; // '0x80'RD9FF50.
code uint8_t ticket_on[9]             = {0x00, 0x52, 0x30, 0x44, 0x38, 0x30, 0x38, 0x30, 0x2E}; // '0x80'R0D8080.

const UINT16 sleep_tbl[8] = {20, 30, 60, 120, 300, 600, 1200, 0};

//------------------------------------------------------------------------
//	Description	: 	hex data to ascii
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 12 - 29
//------------------------------------------------------------------------
uint8_t rf_cmd_judge(uint8_t *cmd_dat, UINT16 cmd_len)
{
	uint8_t cmd_type = CMD_NONE;
	
	switch(cmd_len)
	{
	    case LEN0:
			if(!memcmp(cmd_dat, language_en, LEN0))
			{
				cmd_type = CMD_LEN0_0;
			}
			else if(!memcmp(cmd_dat, language_ge, LEN0))
			{
				cmd_type = CMD_LEN0_1;
			}
			else if(!memcmp(cmd_dat, language_fr, LEN0))
			{
				cmd_type = CMD_LEN0_2;
			}
			else if(!memcmp(cmd_dat, language_it, LEN0))
			{
				cmd_type = CMD_LEN0_3;
			}
			else if(!memcmp(cmd_dat, language_fi, LEN0))
			{
				cmd_type = CMD_LEN0_4;
			}
			else if(!memcmp(cmd_dat, language_es, LEN0))
			{
				cmd_type = CMD_LEN0_5;
			}
			else if(!memcmp(cmd_dat, language_pt, LEN0))
			{
				cmd_type = CMD_LEN0_6;
			}
			else if(!memcmp(cmd_dat, language_nw, LEN0))
			{
				cmd_type = CMD_LEN0_7;
			}
			else if(!memcmp(cmd_dat, language_ru, LEN0))
			{
				cmd_type = CMD_LEN0_8;
			}
			else if(!memcmp(cmd_dat, language_tk, LEN0))
			{
				cmd_type = CMD_LEN0_9;
			}
            break;
	    
		case LEN1:
			if(!memcmp(cmd_dat, bat_volume, LEN1))
			{
				cmd_type = CMD_LEN1_0;
			}
			break;

		case LEN2:
			if(!memcmp(cmd_dat, one_key_pair, LEN2))
			{
				cmd_type = CMD_LEN2_0;
			}
            else if(!memcmp(cmd_dat, user_id_set, LEN2))
			{
				cmd_type = CMD_LEN2_1;
			}
            else if(!memcmp(cmd_dat, user_id_on, LEN2))
			{
				cmd_type = CMD_LEN2_2;
			}
            else if(!memcmp(cmd_dat, user_id_off, LEN2))
			{
				cmd_type = CMD_LEN2_3;
			}
            else if(!memcmp(cmd_dat, set_para_x, LEN2-1))
			{
			    if((cmd_dat[LEN2-1] >= '0') && (cmd_dat[LEN2-1] <= '9'))
			    {
				    cmd_type = CMD_LEN2_4;
				}
			}
			break;

		case LEN3:
			if(!memcmp(cmd_dat, buzzer_open1, LEN3))
			{
				cmd_type = CMD_LEN3_0;
			}
			else if(!memcmp(cmd_dat, buzzer_open2, LEN3))
			{
				cmd_type = CMD_LEN3_1;
			}
			else if(!memcmp(cmd_dat, software_ver, LEN3))
			{
				cmd_type = CMD_LEN3_2;
			}
			else if(!memcmp(cmd_dat, factory_rst, LEN3))
			{
				cmd_type = CMD_LEN3_3;
			}
			else if(!memcmp(cmd_dat, ticket_on, LEN3))
			{
				cmd_type = CMD_LEN3_4;
			}
			else if(!memcmp(cmd_dat, ticket_off, LEN3))
			{
				cmd_type = CMD_LEN3_5;
			}
			break;
		
		case LEN4:
			if(!memcmp(cmd_dat, buzzer_close1, LEN4))
			{
				cmd_type = CMD_LEN4_0;
			}
			else if(!memcmp(cmd_dat, buzzer_close2, LEN4))
			{
				cmd_type = CMD_LEN4_1;
			}
			else if(!memcmp(cmd_dat, power_off, LEN4))
			{
				cmd_type = CMD_LEN4_2;
			}
			else if(!memcmp(cmd_dat, sleep_tim_0x, LEN4-1))
			{
			    if((cmd_dat[LEN4-1] >= '0') && (cmd_dat[LEN4-1] <= '6'))
			    {
				    cmd_type = CMD_LEN4_3;
				}
			}
			break;

		case LEN5:
			if(!memcmp(cmd_dat, sleep_tim_set, LEN5))
			{
				cmd_type = CMD_LEN5_0;
			}
			else if(!memcmp(cmd_dat, sleep_tim_dis, LEN5))
			{
				cmd_type = CMD_LEN5_1;
			}
			else if(!memcmp(cmd_dat, normal_mode, LEN5))
			{
				cmd_type = CMD_LEN5_2;
			}
			else if(!memcmp(cmd_dat, upload_data, LEN5))
			{
				cmd_type = CMD_LEN5_3;
			}
			else if(!memcmp(cmd_dat, total_count, LEN5))
			{
				cmd_type = CMD_LEN5_4;
			}
			else if(!memcmp(cmd_dat, clear_data, LEN5))
			{
				cmd_type = CMD_LEN5_5;
			}
			else if(!memcmp(cmd_dat, hardware_ver, LEN5))
			{
				cmd_type = CMD_LEN5_6;
			}			
			break;

		case LEN6:
			if(!memcmp(cmd_dat, store_mode, LEN6))
			{
				cmd_type = CMD_LEN6_0;
			}
			else if(!memcmp(cmd_dat, unlost_mode, LEN6))
			{
				cmd_type = CMD_LEN6_1;
			}
			break;
		
		default:
			break;
	}

	return cmd_type;
}

//------------------------------------------------------------------------
//	Description	: 	hex data to ascii
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 12 - 29
//------------------------------------------------------------------------
static uint8_t hex_to_ascii(UINT16 hex16, uint8_t *out)
{
	INT8 i;
	uint8_t len;
	uint8_t tmp8;
	UINT16 n;
	uint8_t flag;
	
	n = 10000;
	flag = 0;
	len = 0;
	
	for(i = 5; i > 0; i--)
	{
		tmp8 = (uint8_t)(hex16 / n);
		
		if(0 == flag)
		{
			if((tmp8 != 0) || (1 == i))
			{
				flag = 1;
			}
		}

		if(flag)
		{
			out[len++] = tmp8 + '0';
		}
		
		hex16 = hex16 % n;
		n = n / 10; 							
	}
	
	return len;
}
/*
//------------------------------------------------------------------------
//	Description	: 	hex data to ascii
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 12 - 29
//------------------------------------------------------------------------
static void byte_to_ascii(uint8_t hex8, uint8_t *out)
{
    uint8_t temp;

    temp = (hex8 >> 4) & 0x0F;
    if(temp < 0x0A)
    {
        *out = temp + '0';
    }
    else
    {
        *out = temp - 0x0A + 'A';
    }

    out++;

    temp = hex8 & 0x0F;
    if(temp < 0x0A)
    {
        *out = temp + '0';
    }
    else
    {
        *out = temp - 0x0A + 'A';
    }
}*/

MENU_SET set = {0xFF, 0xFF, 0};

//------------------------------------------------------------------------
//	Description	: 	UART data to usb send
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 03 - 11
//------------------------------------------------------------------------
void set_type_init(SET_TYPE type, MENU_SET *set)
{
	switch(type)
	{
		case SET_NONE:
			set->ch = 0xFF;
			set->id = 0xFF;
			break;

		case SET_CH:
			set->ch = 0xFF;
			break;

		case SET_ID:
			set->id = 0xFF;
			break;

		case SET_BUZZ:
			break;

		default:
			break;
	}

	set->len = 0;
	set->type = type;
	memset(set->dat, 0, SET_BUF);
}

//extern void LangSet(unsigned char lang);

//------------------------------------------------------------------------
//	Description	: 	UART data to usb send
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 03 - 11
//------------------------------------------------------------------------
uint8_t rf_lang_get(uint8_t lang)
{
    uint8_t ret;
    
    if (lang == 0) // US
    {
        ret = 0;
    }
    else if (lang == 10) // GE
    {
        ret = 5;
    }
    else if (lang == 9) // FR
    {
        ret = 4;
    }
    else if (lang == 8) // IT
    {
        ret = 6;
    }
    else if (lang == 3) // ES
    {
        ret = 2;
    }
    else if (lang == 14) // PT
    {
        ret = 3;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

//------------------------------------------------------------------------
//	Description	: 	UART data to usb send
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 03 - 11
//------------------------------------------------------------------------
BOOL rf_host_cmd(uint8_t *cmd_dat)
{
	BOOL flag = TRUE;

	switch(uart.cmd_type)
	{

        default: // Not cmd data
        {
            flag = FALSE;
            
            set.host = 0;
            rf_host.rx_init = FALSE;
            set_type_init(SET_NONE, &set);
            break;
        }
	}

	return flag;
}

