#ifndef	__MG1S02_H__
#define	__MG1S02_H__


#include "Common.h"
#include "BSP.h"


//识读模式
typedef enum
{
	MODE_MANUAL = 0,		//手动模式
	MODE_INDUCTION,			//感应模式
	MODE_CONTINUAL, 		//连续模式
	MODE_COMMAND			//命令触发模式
}
Scan_Mode;

//瞄准
typedef enum
{
	COLLIMATE_NONE = 0,		//无瞄准
	COLLIMATE_NORMAL, 		//普通
	COLLIMATE_ALWAYS		//常亮
}
Collimate_Mode;

//照明
typedef enum
{
	LIGHT_NONE = 0,		//无照明
	LIGHT_NORMAL, 		//普通
	LIGHT_ALWAYS		//常亮
}
Light_Mode;

//有源蜂鸣器模式下默认电平
typedef enum
{
	IDLE_HIGH_LEVEL = 0,	//蜂鸣器空闲高电平，忙碌低电平
	IDLE_LOW_LEVEL			//蜂鸣器空闲低电平，忙碌高电平
}
Default_Level;

//数据编码格式
typedef enum
{
	ENCODING_FORMAT_GBK = 0,	
	ENCODING_FORMAT_UNICODE,	//仅在输出有效
	ENCODING_FORMAT_AUTO,		//仅在输入有效
	ENCODING_FORMAT_UTF8
}
Encoding_Format;

//数据输出模式
typedef enum
{
	OUTPUT_MODE_UART = 0,		//串口输出
	OUTPUT_MODE_USB_PC = 1,		//USB PC键盘
	OUTPUT_MODE_USB_UART = 3	//USB 虚拟串口
}
Output_Mode;

//奇偶校验模式
typedef enum
{
	PARITY_NONE = 0,	//无校验
	PARITY_ODD,			//奇校验
	PARITY_EVEN			//偶校验
}
Parity_Mode;

//解码范围
typedef enum
{
	DECODE_SCOPE_FULL = 0,		//全幅区域
	DECODE_SCOPE_CENTRE_ONLY	//仅中心区域
}
Decode_Scope;

//条码总开关
typedef enum
{
	BARCODE_CONTROL_DISABLE = 0,		//禁止识读所有条码
	BARCODE_CONTROL_ENABLE_ALL = 1,		//允许识读所有条码
	BARCODE_CONTROL_ENABLE_DEFAULT = 2	//打开默认可识读条码
}
Barcode_Control;

//结束符
typedef enum
{
	TAIL_CR = 0,	//回车
	TAIL_CRLF,		//回车换行
	TAIL_TAB,		//TAB
	TAIL_NULL		//无
}
Tail_Type;

//各国键盘
typedef enum
{
	KEYBOARD_AMERICA = 0,	//美国
	KEYBOARD_CZECH,			//捷克
	KEYBOARD_FRANCE,		//法国
	KEYBOARD_GERMANY,		//德国
	KEYBOARD_HUNGARY,		//匈牙利
	KEYBOARD_ITALY,			//意大利
	KEYBOARD_JAPAN,			//日本
	KEYBOARD_SPAIN,			//西班牙
	KEYBOARD_TURKEY_Q,		//土耳其Q
	KEYBOARD_TURKEY_F		//土耳其F
}
Keyboard_Type;

//CodeID列表
typedef enum
{
	CODEID_EAN_13 = 0x91,
	CODEID_EAN_8,
	CODEID_UPC_A,
	CODEID_UPC_E0,
	CODEID_UPC_E1,
	CODEID_CODE_128,
	CODEID_CODE_39,
	CODEID_CODE_93,
	CODEID_CODEBAR,
	CODEID_INTERLEAVED_2_OF_5,
	CODEID_INDUSTRIAL_2_OF_5,
	CODEID_MATTRIX_2_OF_5,
	CODEID_CODE_11,
	CODEID_MIS_PLESSEY,
	CODEID_RSS_14,			
	CODEID_RSS_LIMITED,	
	CODEID_RSS_EXPANDED,
	CODEID_QR_CODE,
	CODEID_DATA_MATTRIX,
	CODEID_PDF_417
}
CodeID_Addr;

//字符截取
typedef enum
{
	INTERCEPT_ALL = 0,				//传送所有Data字符
	INTERCEPT_FIRST,				//仅传送前M个Data字符
	INTERCEPT_LAST,					//仅传送后N个Data字符
	INTERCEPT_WITHOUT_FIRST_LAST	//不传送前M+后N个Data字符
}
Character_Interception;


extern void MG1S02Init(void);		//初始化
extern void MG1S02Scan(void);		//扫描
extern void MG1S02Open(void);
extern void MG1S02Close(void);
extern bool MG1S02AllowRead(void);	//允许读数据
extern void MG1S02ReadData(uint8_t* Data, uint16_t* DataLen);	//读取一个扫码数据
extern bool MG1S02SaveFlag(void);		//保存标志位到该模块的flash 
extern bool MG1S02FlagFactorySetting(void);  	//标志位恢复到出厂设置
extern bool MG1S02SetBaud(uint32_t Baud);		//设置波特率
extern bool MG1S02GetBaud(uint32_t* Baud);		//获得波特率
extern bool MG1S02SendCommand(uint16_t FlagAddr, uint8_t FlagLen, uint8_t BitsLen, uint8_t BitsOffset, uint16_t Data ,bool NeedToRead);		//发送串口命令
extern bool MG1S02GetSetting(uint16_t FlagAddr, uint8_t FlagLen, uint8_t BitsLen, uint8_t BitsOffset, uint16_t* Value);			//获取指定标志位值
extern bool MG1S02SetCharacterValue(uint32_t Addr, const char *Str, uint16_t Len);		//设置字符值
extern bool MG1S02GetCharacterValue(uint32_t Addr, char *Str, uint16_t Len);		//获取字符值
extern bool MG1S02GetVersion(uint32_t Addr, char* Version);		//获取版本号
extern bool MG1S02GetSoftwareDate(char* Date);		//获取软件日期
extern bool MG1S02GetProductModel(char* Model);		//获取产品型号

#endif

