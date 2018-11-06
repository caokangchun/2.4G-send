#ifndef __CONFIGADDR_H__
#define __CONFIGADDR_H__	1



#define MCU_FLASH_SIZE	(128*1024)

#define EEPROM_SIZE		(1*1024)


/*以下#define用于定义配置在eeprom中的偏移地址*/



#define CONFIG_NoConfig										(0)

#define CONFIG_2_4MAC										(CONFIG_NoConfig+2)		//+1个半字

#define CONFIG_ScanMode										(CONFIG_2_4MAC+1)		//+2个半字

#define CONFIG_Keyboard										(CONFIG_ScanMode+1)

#define CONFIG_BeepEn										(CONFIG_Keyboard+1)

#define CONFIG_Sleep										(CONFIG_BeepEn+1)

#define CONFIG_SuccessCount									(CONFIG_Sleep+2)






#endif
