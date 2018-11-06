/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : All processings related to Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "MM32F103.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;
uint8_t Report_Buf[256];
uint16_t Report_Ptr = 0;
uint16_t IdleRate = 0;
/* Private variables ---------------------------------------------------------*/
uint8_t Request = 0;

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
  };

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table[2] =
{
  {
    EP_NUM_HID,
    1
  },
  {
    EP_NUM_VCP,
    1
  } 
};

DEVICE_PROP Device_Property[2] =
{
  {
    CustomHID_init,
    CustomHID_Reset,
    CustomHID_Status_In,
    CustomHID_Status_Out,
    CustomHID_Data_Setup,
    CustomHID_NoData_Setup,
    CustomHID_Get_Interface_Setting,
    CustomHID_GetDeviceDescriptor,
    CustomHID_GetConfigDescriptor,
    CustomHID_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  },
  {
    Virtual_Com_Port_init,
    Virtual_Com_Port_Reset,
    Virtual_Com_Port_Status_In,
    Virtual_Com_Port_Status_Out,
    Virtual_Com_Port_Data_Setup,
    Virtual_Com_Port_NoData_Setup,
    Virtual_Com_Port_Get_Interface_Setting,
    Virtual_Com_Port_GetDeviceDescriptor,
    Virtual_Com_Port_GetConfigDescriptor,
    Virtual_Com_Port_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  }
};

USER_STANDARD_REQUESTS User_Standard_Requests[2] =
{
  {
    CustomHID_GetConfiguration,
    CustomHID_SetConfiguration,
    CustomHID_GetInterface,
    CustomHID_SetInterface,
    CustomHID_GetStatus,
    CustomHID_ClearFeature,
    CustomHID_SetEndPointFeature,
    CustomHID_SetDeviceFeature,
    CustomHID_SetDeviceAddress
  },
  {
    Virtual_Com_Port_GetConfiguration,
    Virtual_Com_Port_SetConfiguration,
    Virtual_Com_Port_GetInterface,
    Virtual_Com_Port_SetInterface,
    Virtual_Com_Port_GetStatus,
    Virtual_Com_Port_ClearFeature,
    Virtual_Com_Port_SetEndPointFeature,
    Virtual_Com_Port_SetDeviceFeature,
    Virtual_Com_Port_SetDeviceAddress
  }
};

ONE_DESCRIPTOR Device_Descriptor[2] =
{
  {
    (uint8_t*)CustomHID_DeviceDescriptor,
    CUSTOMHID_SIZ_DEVICE_DESC
  },
  {
    (uint8_t*)Virtual_Com_Port_DeviceDescriptor,
    VIRTUAL_COM_PORT_SIZ_DEVICE_DESC
  },
};

ONE_DESCRIPTOR Config_Descriptor[2] =
{
  {
    (uint8_t*)CustomHID_ConfigDescriptor,
    CUSTOMHID_SIZ_CONFIG_DESC
  },
  {
    (uint8_t*)Virtual_Com_Port_ConfigDescriptor,
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC
  }
};

ONE_DESCRIPTOR CustomHID_Report_Descriptor[2] =
{
  {
    (uint8_t *)CustomHID_ReportKDescriptor,
    CUSTOMHID_SIZ_REPORT_K_DESC
  },
  {
    (uint8_t *)CustomHID_ReportCDescriptor,
    CUSTOMHID_SIZ_REPORT_C_DESC
  }
};

ONE_DESCRIPTOR CustomHID_Hid_Descriptor[2] =
{
  {
    (uint8_t*)CustomHID_ConfigDescriptor + CUSTOMHID_OFF_HID_K_DESC,
    CUSTOMHID_SIZ_HID_DESC
  },
  {
    (uint8_t*)CustomHID_ConfigDescriptor + CUSTOMHID_OFF_HID_C_DESC,
    CUSTOMHID_SIZ_HID_DESC
  },
};

ONE_DESCRIPTOR String_Descriptor[2][4] =
{
  {
    {(uint8_t*)CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID},
    {(uint8_t*)CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR},
    {(uint8_t*)CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT},
    {(uint8_t*)CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL}
  },
  {
    {(uint8_t*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID},
    {(uint8_t*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR},
    {(uint8_t*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT},
    {(uint8_t*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL}
  }
};

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CustomHID_init.
* Description    : Custom HID init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_init(void)
{
  /* Update the serial number string descriptor with the data from the unique 
  ID*/
  Get_SerialNum();
    
  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();
  /* USB interrupts initialization */

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : CustomHID_Reset.
* Description    : Custom HID reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Reset(void)
{
    /* Set Joystick_DEVICE as not configured */
    pInformation->Current_Configuration = 0;
    pInformation->Current_Interface = 0;/*the default Interface*/

    /* Current Feature initialization */
    pInformation->Current_Feature = CustomHID_ConfigDescriptor[7];

    /* Set this device to response on default address */

    _ClrEP_EN((1<<ENDP0)|(1<<ENDP1)|(1<<ENDP2));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | \
                    EPn_INT_EN_INACKIE | EPn_INT_EN_OUTACKIE);
    _ClrEP0_INT_STA( EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK|\
                    EPn_INT_STATE_INACK | EPn_INT_STATE_OUTACK);

    _SetEP1_INT_EN( EPn_INT_EN_INACKIE );// | EPn_INT_EN_ENDIE);
    _ClrEP1_INT_STA( EPn_INT_STATE_INACK );// | EPn_INT_STATE_END);
	
	 _SetEP2_INT_EN( EPn_INT_EN_INACKIE );
	_ClrEP2_INT_STA( EPn_INT_STATE_INACK|EPn_INT_STATE_OUTACK );
	
    _SetEP_EN((1<<ENDP0)|(1<<ENDP1)|(1<<ENDP2));
    _SetEP_INT_EN((1<<ENDP0)|(1<<ENDP1)|(1<<ENDP2));
    SetDeviceAddress(0);

    _SetUSB_TOP(USB_TOP_RESET);
    _ClrUSB_TOP(USB_TOP_RESET); 

    bDeviceState = ATTACHED;
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Udpade the device state to configured and command the ADC 
*                  conversion.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : CustomHID_Status_In.
* Description    : Joystick status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_In(void)
{
}

/*******************************************************************************
* Function Name  : CustomHID_Status_Out
* Description    : Joystick status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_Out (void)
{
}

/*******************************************************************************
* Function Name  : CustomHID_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;

  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 < 2))
  {

    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = CustomHID_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = CustomHID_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL, GET_REPORT, SET_REPORT ***/
  else if ( (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) )
  {         
    switch( RequestNo )
    {
    case GET_PROTOCOL:
      CopyRoutine = CustomHID_GetProtocolValue;
      break;
    case SET_REPORT:
      CopyRoutine = CustomHID_SetReport;
      break;
    case GET_IDLE:
      CopyRoutine = CustomHID_GetIdleValue;
      break;
    case GET_REPORT:
      CopyRoutine = CustomHID_GetReport;
      break;
    default:
      break;
    }
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetReport
* Description    : Set Feature request handling
* Input          : Length.
* Output         : None.
* Return         : Buffer
*******************************************************************************/
uint8_t *CustomHID_SetReport(uint16_t Length)
{
  if (Length == 0)
  {
    Report_Ptr = 0;
    pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w;
    return NULL;
  }
  else
  {
    uint8_t *buf_ptr = NULL;
    
    buf_ptr = &Report_Buf[Report_Ptr];
    Report_Ptr += Length;
    return buf_ptr;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_SetReport
* Description    : Set Feature request handling
* Input          : Length.
* Output         : None.
* Return         : Buffer
*******************************************************************************/
uint8_t *CustomHID_GetReport(uint16_t Length)
{
  extern void usb_report_in(uint8_t *rpt_buf);
  
  if (Length == 0)
  {
    usb_report_in(Report_Buf);
    pInformation->Ctrl_Info.Usb_wLength = 15;//256;
    return NULL;
  }
  else
  {
    return Report_Buf;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_NoData_Setup(uint8_t RequestNo)
{
  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
      if(RequestNo == SET_PROTOCOL)
      {
        return CustomHID_SetProtocol();
      }
      else if(RequestNo == SET_IDLE)
      {
        return CustomHID_SetIdle();
      }
      else
        return USB_UNSUPPORT;
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor[0]);
}

/*******************************************************************************
* Function Name  : CustomHID_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor[0]);
}

/*******************************************************************************
* Function Name  : CustomHID_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *CustomHID_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else 
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[0][wValue0]);
  }
}

/*******************************************************************************
* Function Name  : CustomHID_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor[pInformation->USBwIndex0]);
}

/*******************************************************************************
* Function Name  : CustomHID_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor[pInformation->USBwIndex0]);
}

/*******************************************************************************
* Function Name  : CustomHID_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT CustomHID_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protcol value.
*******************************************************************************/
uint8_t *CustomHID_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/*******************************************************************************
* Function Name  : CustomHID_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT CustomHID_SetIdle(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  IdleRate = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t *CustomHID_GetIdleValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 2;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&IdleRate);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_init.
* Description    : Virtual COM Port Mouse init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();

  /* configure the USART to the default settings */
  //USART_Config_Default();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Reset
* Description    : Virtual_Com_Port Mouse reset routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Reset(void)
{
  /* Set Virtual_Com_Port DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = Virtual_Com_Port_ConfigDescriptor[7];

  /* Set Virtual_Com_Port DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;

  _ClrEP_EN((1<<ENDP0)|(1<<ENDP1)|(1<<ENDP2)|(1<<ENDP3));
  
  _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | \
                  /*EPn_INT_EN_INACKIE | */EPn_INT_EN_OUTACKIE);
  _ClrEP0_INT_STA( EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK|\
                  /*EPn_INT_STATE_INACK | */EPn_INT_STATE_OUTACK);
  
  _SetEP1_INT_EN( EPn_INT_EN_INACKIE );
  _ClrEP1_INT_STA( EPn_INT_STATE_INACK );

    _SetEP2_INT_EN( EPn_INT_EN_INACKIE );
  _ClrEP2_INT_STA( EPn_INT_STATE_INACK );
  
    _SetEP3_INT_EN( EPn_INT_EN_OUTACKIE );
  _ClrEP3_INT_STA( EPn_INT_STATE_OUTACK );
  
  _SetEP_EN((1<<ENDP0)|(1<<ENDP1)|(1<<ENDP2)|(1<<ENDP3));
  _SetEP_INT_EN((1<<ENDP0)|(1<<ENDP1)|(1<<ENDP2)|(1<<ENDP3));
    
  _SetUSB_TOP(USB_TOP_RESET);
  _ClrUSB_TOP(USB_TOP_RESET);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  
  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_In.
* Description    : Virtual COM Port Status In Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_In(void)
{
  if (Request == SET_LINE_CODING)
  {
    //USART_Config();
    Request = 0;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_Out
* Description    : Virtual COM Port Status OUT Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_Out(void)
{}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Data_Setup
* Description    : handle the data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Com_Port_Data_Setup(uint8_t RequestNo)
{
  uint8_t    *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;

  if (RequestNo == GET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = Virtual_Com_Port_GetLineCoding;
    }
  }
  else if (RequestNo == SET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = Virtual_Com_Port_SetLineCoding;
    }
    Request = SET_LINE_CODING;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_NoData_Setup.
* Description    : handle the no data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Com_Port_NoData_Setup(uint8_t RequestNo)
{

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    if (RequestNo == SET_COMM_FEATURE)
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)
    {
      return USB_SUCCESS;
    }
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *Virtual_Com_Port_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor[1]);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetConfigDescriptor.
* Description    : get the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Virtual_Com_Port_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor[1]);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Virtual_Com_Port_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[1][wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.
* Input1         : uint8_t: Interface : interface number.
* Input2         : uint8_t: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Virtual_Com_Port_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 1)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetLineCoding.
* Description    : send the linecoding structure to the PC host.
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.
*******************************************************************************/
uint8_t *Virtual_Com_Port_GetLineCoding(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(uint8_t *)&linecoding;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetLineCoding.
* Description    : Set the linecoding structure fields.
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.
*******************************************************************************/
uint8_t *Virtual_Com_Port_SetLineCoding(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(uint8_t *)&linecoding;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
