#include "EncodeDataExtract.h"


short int ExtractBit16(uint32_t Address)
{
    uint16_t x;
    uint8_t y;
    Flash_Read_Data(Address,1,&y);
    x=y;
    Address++;
    Flash_Read_Data(Address,1,&y);
    x=x+(y<<8);
    return x;
//    Address=1662;
//    while(1)
//    {
//        Flash_Read_Data(Address,1,&y);
//        x=y;
//        Address++;        
//    }
}

Summary16* ExtractSummary16(uint32_t Address)
{
//    static Summary16 buff;          
//    uint8_t x;
//    Flash_Read_Data(Address,1,&x);
//    buff.indx=x;
//    Address++;
//    Flash_Read_Data(Address,1,&x);  
//    buff.indx=(buff.indx<<8)+x;
//    Address++;
//    
//    Flash_Read_Data(Address,1,&x);
//    buff.used=x;
//    Address++;
//    Flash_Read_Data(Address,1,&x);  
//    buff.used=(buff.used<<8)+x;  
    static Summary16 buff;
    uint8_t* buff1=(uint8_t *)(&buff);
    Flash_Read_Data(Address,1,buff1);
    Address++;
    buff1++;
    Flash_Read_Data(Address,1,buff1);
    Address++;
    buff1++;    
    Flash_Read_Data(Address,1,buff1);
    Address++;
    buff1++;  
    Flash_Read_Data(Address,1,buff1);
    return &buff;
}

char ExtractBit8(uint32_t Address)
{
    uint8_t x;
    Flash_Read_Data(Address,1,&x);
    return x;
}
