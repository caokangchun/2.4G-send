#ifndef	__SCANACTION_H__
#define	__SCANACTION_H__


#include <stdint.h>
#include "Common.h"
#include "BSP.h"


/**
//EEPROM内存地址预定义
**/

#define EEPROM_FIRST_USE				0x01			//0x800fc00

#define EEPROM_DEV_ADDR					0x03
#define EEPROM_USER_ADDR				(EEPROM_DEV_ADDR+SET_SIZE)



#define ADD_CODE_NUM					15											//最多允许15位前缀
#define PRE_CODE_NUM					(ADD_CODE_NUM+6+9)         					//放置“2位预留位 + 1位STX + 3位用户ID + 15位前缀+9位预留”,
#define SUF_CODE_NUM    				(ADD_CODE_NUM+6+9)         					//放置“15位后缀 + 1位ETX + 2位结束符 + 3位预留位+9位预留”,
#define INSERT_GROUP					8											//插入组数
#define INSERT_NUM						10											//可插入长度
#define DELETE_NUM						10											//可删除长度
#define REPLACE_NUM						10											//可替换长度
#define PRIOR_CODE_NUM					(ADD_CODE_NUM+INSERT_GROUP*INSERT_NUM)		//95


//功能状态
typedef enum
{	
	OFF,	
	ON	
} 
Status_type; 

//设置状态
typedef enum
{	
	NoSet,				//无
	Hid_Pre,			//前隐藏
	Hid_Suf,			//后隐藏
	Keep_Pre,			//前保留
	Keep_Suf,			//后保留
	Add_Pre,			//添加前缀
	Add_Suf,			//添加后缀
			
	Insert_Group,		//插入组
	InsertCodeBuf,		//插入字符
	Limit_Lower,		//最小长度
	Limit_Upper,		//最大长度
	User_ID,			//用户ID
	Delete,				//删除
			
	Replace_Old,		//替换_旧
	Replace_New,		//替换_新
	Sleep_Time,			//休眠时间
	Inventory_Time		//
	
}
Setting_Type;	

//音量大小
typedef enum
{	
	BEEP_VOL_SMALL = 10,	
	BEEP_VOL_BIG = 50,	
}
BeepVol_type;	

//音长
typedef enum
{	
	BEEP_TONE_LONG = 1000,	
	BEEP_TONE_CENTRE = 2000,	
	BEEP_TONE_SHORT = 3000,
}
Beep_Change;	

//蜂鸣声
typedef enum
{	
	Normal_Beep,		//正常
	Setting_Beep		//设置
}	
Beep_Type;	

//波特率
typedef enum
{	
	BaudRate_1200=1200,	
	BaudRate_2400=2400,	
	BaudRate_4800=4800,	
	BaudRate_9600=9600,
	BaudRate_19200=19200,	
	BaudRate_38400=38400,
	BaudRate_57600=57600,	
	BaudRate_115200=115200
} 
BaudRate_Type;	

//语言
typedef enum
{	
	America,				//美国英语
	France,					//法国法语
	German,					//德国德语
	France_CAN,				//加拿大法语
	Dutch,					//荷兰语
	Spanish,				//西班牙语
	Portuguese_BR,			//巴西（葡萄牙语）
	Italian,				//意大利语
	Portuguese,				//葡萄牙语
	Turkish_F,				//土耳其F
	Turkish_Q,				//土耳其Q
	Japanese,				//日本
	Russian					//俄罗斯
} 
Language_Type;	

//瞄准_亮度设置
typedef enum
{	
	NONE = 0,				//无瞄准
	NORMAL, 				//普通
	ALWAYS					//常亮
}	
Sight_LightSet;	

//字母状态
typedef enum
{	
	Normal,					//正常
	Capital,				//大写
	Lowercase,				//小写
	Inversion				//反转
}	
LetterStatus_Type;	

//后缀结束符
typedef enum
{
	SUF_TAIL_CR = 0,	//回车
	SUF_TAIL_CRLF,		//回车换行
	SUF_TAIL_TAB,		//TAB
	SUF_TAIL_NULL		//无
}
Suf_Tail;	

//连续扫描间隔时长
typedef enum
{
	INTERVAL_0MS=0,
	INTERVAL_500MS=500,
	INTERVAL_1000MS=1000,
	INTERVAL_1500MS=1500,
	INTERVAL_2000MS=2000
}
Interval_type;

//数据位
typedef enum
{
	DATABIT_7BIT=7,
	DATABIT_8BIT=8
}
DataBit_type;	

//停止位
typedef enum
{
	STOPBIT_1BIT=1,
	STOPBIT_2BIT=2
}
StopBit_type;	

//奇偶校验模式
typedef enum
{
	PARITYBIT_NONE = 0,		//无校验
	PARITYBIT_ODD,			//奇校验
	PARITYBIT_EVEN			//偶校验
}
ParityBit_type;

//识读模式
typedef enum
{
	_MODE_MANUAL = 0,		//手动模式
	_MODE_INDUCTION,		//感应模式
	_MODE_CONTINUAL, 		//连续模式
	_MODE_COMMAND			//命令触发模式
}
ScanMode_type;

//上传模式
typedef enum
{
	Normal_Mode,			//普通模式
	Inventory_Mode			//盘点模式
}
UploadMode_type;

//接口
typedef enum
{
	Usart_Mode,				//串口
	USB_Mode,				//USB
	USB_VCP_Mode,			//USB虚拟串口
	Wireless_Mode			//无线
}
TxMode_Type;	

typedef struct 
{
	uint8_t uState;					//开关状态
	uint8_t uStartStopSymbol;		//是否发送起止符
	uint8_t uMiniLen;				//最短长度
	uint8_t uMaxLen;				//最长长度
}
SetCode_type;

typedef struct 
{
	//蜂鸣器
	Status_type	BeepSound;							//蜂鸣器声音 1:开 0：关
	Status_type StartSound;							//开机声音	1:开 0：关
	BeepVol_type BeepVol;							//蜂鸣器音量 
	Beep_Change BeepLenght;							//蜂鸣器音调	
	
	ScanMode_type ScanMode;							//扫描模式
	Suf_Tail ScanTail;								//扫描后缀、结束符（后缀）
	Interval_type ScanInterval;						//连续扫描间隔 //Interval = ScanInterval*100;
	UploadMode_type UploadMode;						//上传模式
	uint8_t InventoryData;							//盘点数据
	TxMode_Type Tx_Mode;							//接口模式
	
	BaudRate_Type BaudRate;							//串口波特率
	DataBit_type DataBit;							//串口数据位	默认8位
	StopBit_type StopBit;							//串口停止位	默认1位
	ParityBit_type ParityBit;						//串口校验位	默认无
	
	
	Sight_LightSet Collimation;						//瞄准设置	默认普通
	Sight_LightSet Lighting;						//照明灯设置	默认普通
	
	
	Language_Type KeyBoardType;						//键盘语言
	
	uint8_t UserIDVar;								//用户ID的值，设置为数范围00~99.
	Status_type CodeType;							//显示条码类型开启或关闭
	Status_type UserID;								//UserID开关
	Status_type IDPre;								//前ID开关
	Status_type IDSuf;								//后ID开关
	Status_type STX;								//STX开关
	Status_type ETX;								//ETX开关
	Status_type HidePre;							//前隐藏开关
	Status_type HideSuf;							//后隐藏开关
	Status_type KeepPre;							//前保留开关
	Status_type KeepSuf;							//后保留开关
	Status_type AddPre;								//前缀添加开关
	Status_type AddSuf;								//后缀添加开关
	Status_type String_Inversion;					//字符倒序开关
	LetterStatus_Type Letter_Status;				//字母大小写状态

	Status_type Insert;								//插入开关
	Status_type Delete;								//删除开关
	Status_type Replace;							//替换开关
	
	uint8_t LimitLowerLen;							//限制上限
	uint16_t LimitUpperLen;							//限制下限
	uint8_t AddPreLen;								//添加前缀长度
	uint8_t AddSufLen;								//添加后缀长度
	uint8_t HidePreLen;								//前隐藏长度
	uint8_t HideSufLen;								//后隐藏长度
	uint16_t KeepPreLen;							//前保留长度
	uint16_t KeepSufLen;							//后保留长度
	uint8_t DeleteLen;								//删除长度
	uint8_t ReplaceOldLen;							//替换前长度
	uint8_t ReplaceNewLen;							//替换后长度
	
	uint8_t AddPreCodeBuf[ADD_CODE_NUM];             //前缀要添加的字符内容（最多15个字符）
	uint8_t AddSufCodeBuf[ADD_CODE_NUM];             //后缀要添加的字符内容（最多15个字符）
	uint8_t InsertAddr[INSERT_GROUP];                //从第几位插入
	uint8_t InsertLen[INSERT_GROUP];                 //共插入几位
	uint8_t InsertCodeBuf[INSERT_GROUP][INSERT_NUM]; //要插入的字符内容（最多插入八组，每组最多含10个字符）
	uint8_t DeleteCodeBuf[DELETE_NUM];               //要删除的字符内容（最多10个字符）
	uint8_t ReplaceOldCodeBuf[REPLACE_NUM];          //替代前的字符内容（最多10个字符）
	uint8_t ReplaceNewCodeBuf[REPLACE_NUM];	         //替代后的字符内容（最多10个字符）
	
	
	
	SetCode_type EAN13;
	SetCode_type EAN8;
	SetCode_type UPCA;
	SetCode_type UPCE0;
	SetCode_type UPCE1;
	
	SetCode_type Code128;
	SetCode_type Code39;
	SetCode_type Code93;
	SetCode_type CodeBar;
	SetCode_type QR;
	SetCode_type Interleaved2of5;
	SetCode_type Industrial25;
	SetCode_type Matrix2of5;
	SetCode_type MSI;
	SetCode_type Code11;	

	
}Dev_Config;


typedef struct
{
	uint8_t PreNum;									//前缀数量
	uint8_t SufNum; 								//后缀数量
	uint8_t PriorNum;								//插入的数量
	uint16_t uNum;     								//总数量
	uint16_t uStart;            					//数据起始位置
	uint8_t PreCodeBuf[PRE_CODE_NUM];               //放置“2位预留位 + 1位STX + 3位用户ID + 15位前缀”,共106位
	uint8_t PriorCodeBuf[PRIOR_CODE_NUM];           //放置扫描出来的条码字符中前99位以及中间的插入字符,共180位
	uint8_t SufCodeBuf[SUF_CODE_NUM];	            //放置“15位后缀 + 1位ETX + 2位结束符 + 3位预留位”,共106位


}MyString_Typedef;


extern Dev_Config ScanSet;


//功能设置初始化，未初始化过就进行出厂设置，设置过就读取
extern void ActionConfigSetInit(void);
//功能配置初始化，用于恢复出厂设置配置
extern void ActionConfigDevInit(void);
//设置码处理
extern uint8_t ActionConfigCodeProcess(uint8_t *BuffCode, uint16_t len);




#endif

