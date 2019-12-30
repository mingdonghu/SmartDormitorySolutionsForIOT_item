/**
************************************************************
* @file         gizwits_protocol.h
* @brief        Corresponding gizwits_product.c header file (including product hardware and software version definition)
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#ifndef _GIZWITS_PROTOCOL_H
#define _GIZWITS_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

                                                                                                                  
#define SEND_MAX_TIME       200                     ///< 200ms resend
#define SEND_MAX_NUM        2                       ///< resend times
                                                    
#define protocol_VERSION    "00000004"              ///< protocol version
#define P0_VERSION          "00000002"              ///< P0 protocol version

/**@name Product Key  
* @{
*/
#define PRODUCT_KEY "fe524f9499f94e22a867f1ad1088d57b"
/**@} */
/**@name Product Secret  
* @{
*/
#define PRODUCT_SECRET "5b709196d9f340efaf805d9d84e6d2ab"
               
/**@name Device status data reporting interval
* @{
*/
#define REPORT_TIME_MAX 6000 //6S
/**@} */    

#define CELLNUMMAX 7    


/**@name Whether the device is in the control class, 0 means no, 1 means yes
* @{
*/
#define DEV_IS_GATEWAY   0                    
/**@} */

/**@name Binding time
* @{
*/
#define NINABLETIME  0
/**@} */



#define MAX_PACKAGE_LEN    (sizeof(devStatus_t)+sizeof(attrFlags_t)+20)                 ///< Data buffer maximum length
#define RB_MAX_LEN          (MAX_PACKAGE_LEN*2)     ///< Maximum length of ring buffer

/**@name Data point related definition
* @{
*/
#define LED_BYTEOFFSET                    0
#define LED_BITOFFSET                     0
#define LED_LEN                           1
#define IS_GetUpAlarm_BYTEOFFSET                    0
#define IS_GetUpAlarm_BITOFFSET                     1
#define IS_GetUpAlarm_LEN                           1
#define IS_GoToBedAlarm_BYTEOFFSET                    0
#define IS_GoToBedAlarm_BITOFFSET                     2
#define IS_GoToBedAlarm_LEN                           1
#define FireMonitorState_BYTEOFFSET                    0
#define FireMonitorState_BITOFFSET                     3
#define FireMonitorState_LEN                           1
#define PowerMonitorState_BYTEOFFSET                    0
#define PowerMonitorState_BITOFFSET                     4
#define PowerMonitorState_LEN                           1
#define FireMonitor_BYTEOFFSET                    7
#define FireMonitor_BITOFFSET                     0
#define FireMonitor_LEN                           1
#define LED_status_BYTEOFFSET                    7
#define LED_status_BITOFFSET                     1
#define LED_status_LEN                           1
#define PowerMonitor_BYTEOFFSET                    7
#define PowerMonitor_BITOFFSET                     2
#define PowerMonitor_LEN                           1

#define SetGetUpHour_RATIO                         1
#define SetGetUpHour_ADDITION                      0
#define SetGetUpHour_MIN                           0
#define SetGetUpHour_MAX                           23
#define SetGetUpMinute_RATIO                         1
#define SetGetUpMinute_ADDITION                      0
#define SetGetUpMinute_MIN                           0
#define SetGetUpMinute_MAX                           59
#define SetGoToBedHour_RATIO                         1
#define SetGoToBedHour_ADDITION                      0
#define SetGoToBedHour_MIN                           0
#define SetGoToBedHour_MAX                           23
#define SetGoToBedMinute_RATIO                         1
#define SetGoToBedMinute_ADDITION                      0
#define SetGoToBedMinute_MIN                           0
#define SetGoToBedMinute_MAX                           59
#define SetPowerMonitorVlaue_RATIO                         1
#define SetPowerMonitorVlaue_ADDITION                      0
#define SetPowerMonitorVlaue_MIN                           600
#define SetPowerMonitorVlaue_MAX                           2000
#define DisplayPowerMonitorVlaue_RATIO                         1
#define DisplayPowerMonitorVlaue_ADDITION                      600
#define DisplayPowerMonitorVlaue_MIN                           0
#define DisplayPowerMonitorVlaue_MAX                           1400
/**@} */

/** Writable data points Boolean and enumerated variables occupy byte size */
#define COUNT_W_BIT 1

/** Read-only data points Boolean and enumerated variables occupy byte size */
#define COUNT_R_BIT 1





typedef enum
{
    FireMonitorState_VALUE0 = 0,//工作
    FireMonitorState_VALUE1 = 1,//复位
    FireMonitorState_VALUE_MAX,
} FireMonitorState_ENUM_T;
typedef enum
{
    PowerMonitorState_VALUE0 = 0,//工作
    PowerMonitorState_VALUE1 = 1,//复位
    PowerMonitorState_VALUE_MAX,
} PowerMonitorState_ENUM_T;
typedef enum
{
    FireMonitor_VALUE0 = 0,//有火灾
    FireMonitor_VALUE1 = 1,//无火灾
    FireMonitor_VALUE_MAX,
} FireMonitor_ENUM_T;
typedef enum
{
    LED_status_VALUE0 = 0,//已开灯
    LED_status_VALUE1 = 1,//已关灯
    LED_status_VALUE_MAX,
} LED_status_ENUM_T;
typedef enum
{
    PowerMonitor_VALUE0 = 0,//不存在大功率
    PowerMonitor_VALUE1 = 1,//存在大功率
    PowerMonitor_VALUE_MAX,
} PowerMonitor_ENUM_T;

/** Event enumeration */
typedef enum
{
  WIFI_SOFTAP = 0x00,                               ///< WiFi SOFTAP configuration event
  WIFI_AIRLINK,                                     ///< WiFi module AIRLINK configuration event
  WIFI_STATION,                                     ///< WiFi module STATION configuration event
  WIFI_OPEN_BINDING,                                ///< The WiFi module opens the binding event
  WIFI_CLOSE_BINDING,                               ///< The WiFi module closes the binding event
  WIFI_CON_ROUTER,                                  ///< The WiFi module is connected to a routing event
  WIFI_DISCON_ROUTER,                               ///< The WiFi module has been disconnected from the routing event
  WIFI_CON_M2M,                                     ///< The WiFi module has a server M2M event
  WIFI_DISCON_M2M,                                  ///< The WiFi module has been disconnected from the server M2M event
  WIFI_OPEN_TESTMODE,                               ///< The WiFi module turns on the test mode event
  WIFI_CLOSE_TESTMODE,                              ///< The WiFi module turns off the test mode event
  WIFI_CON_APP,                                     ///< The WiFi module connects to the APP event
  WIFI_DISCON_APP,                                  ///< The WiFi module disconnects the APP event
  WIFI_RSSI,                                        ///< WiFi module RSSI event
  WIFI_NTP,                                         ///< Network time event
  MODULE_INFO,                                      ///< Module information event
  TRANSPARENT_DATA,                                 ///< Transparency events
  EVENT_LED,
  EVENT_IS_GetUpAlarm,
  EVENT_IS_GoToBedAlarm,
  EVENT_FireMonitorState,
  EVENT_PowerMonitorState,
  EVENT_SetGetUpHour,
  EVENT_SetGetUpMinute,
  EVENT_SetGoToBedHour,
  EVENT_SetGoToBedMinute,
  EVENT_SetPowerMonitorVlaue,
  EVENT_TYPE_MAX                                    ///< Enumerate the number of members to calculate (user accidentally deleted)
} EVENT_TYPE_T;

/** P0 Command code */
typedef enum
{
    ACTION_CONTROL_DEVICE       = 0x01,             ///< Protocol 4.10 WiFi Module Control Device WiFi Module Send
    ACTION_READ_DEV_STATUS      = 0x02,             ///< Protocol 4.8 WiFi Module Reads the current status of the device WiFi module sent
    ACTION_READ_DEV_STATUS_ACK  = 0x03,             ///< Protocol 4.8 WiFi Module Read Device Current Status Device MCU Reply
    ACTION_REPORT_DEV_STATUS    = 0x04,             ///< Protocol 4.9 device MCU to the WiFi module to actively report the current status of the device to send the MCU
    ACTION_W2D_TRANSPARENT_DATA = 0x05,             ///< WiFi to device MCU transparent
    ACTION_D2W_TRANSPARENT_DATA = 0x06,             ///< Device MCU to WiFi
} actionType_t;   

/** Protocol network time structure */
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t ntp;
}protocolTime_t;
           

/** WiFi Module configuration parameters*/
typedef enum
{
  WIFI_RESET_MODE = 0x00,                           ///< WIFI module reset
  WIFI_SOFTAP_MODE,                                 ///< WIFI module softAP modeF
  WIFI_AIRLINK_MODE,                                ///< WIFI module AirLink mode
  WIFI_PRODUCTION_TEST,                             ///< MCU request WiFi module into production test mode
  WIFI_NINABLE_MODE,                                ///< MCU request module to enter binding mode
  WIFI_REBOOT_MODE,                                 ///< MCU request module reboot  
}WIFI_MODE_TYPE_T;                                

/** The protocol event type*/
typedef enum
{
  STATELESS_TYPE = 0x00,                            ///< Stateless type
  ACTION_CONTROL_TYPE,                              ///< Protocol 4.10 :WiFi module control device event
  WIFI_STATUS_TYPE,                                 ///< Protocol 4.5 :WiFi module inform the device MCU of the change event of the WiFi module status
  ACTION_W2D_TRANSPARENT_TYPE,                      ///< Protocol WiFi to device MCU transparent event
  GET_NTP_TYPE,                                     ///< Protocol 4.13 :The MCU requests access to the network time event
  GET_MODULEINFO_TYPE,                              ///< Protocol 4.9 :The MCU get module information event
  PROTOCOL_EVENT_TYPE_MAX                           ///< Count enumerated member (User donot delete)
} PROTOCOL_EVENT_TYPE_T;
     
/** Protocol command code */                                   
typedef enum                               
{                                                   
    CMD_GET_DEVICE_INTO             = 0x01,         ///< Protocol：3.1 
    ACK_GET_DEVICE_INFO             = 0x02,         ///< Protocol：3.1 
            
    CMD_ISSUED_P0                   = 0x03,         ///< Protocol：3.2 3.3 
    ACK_ISSUED_P0                   = 0x04,         ///< Protocol：3.2 3.3 
            
    CMD_REPORT_P0                   = 0x05,         ///< Protocol：3.4 
    ACK_REPORT_P0                   = 0x06,         ///< Protocol：3.4 
            
    CMD_HEARTBEAT                   = 0x07,         ///< Protocol：3.5 
    ACK_HEARTBEAT                   = 0x08,         ///< Protocol：3.5 
            
    CMD_WIFI_CONFIG                 = 0x09,         ///< Protocol：3.6 
    ACK_WIFI_CONFIG                 = 0x0A,         ///< Protocol：3.6 
            
    CMD_SET_DEFAULT                 = 0x0B,         ///< Protocol：3.7 
    ACK_SET_DEFAULT                 = 0x0C,         ///< Protocol：3.7 
            
    CMD_WIFISTATUS                  = 0x0D,         ///< Protocol：3.8 
    ACK_WIFISTATUS                  = 0x0E,         ///< Protocol：3.8 
        
    CMD_MCU_REBOOT                  = 0x0F,         ///< Protocol：4.1 
    ACK_MCU_REBOOT                  = 0x10,         ///< Protocol：4.1 
            
    CMD_ERROR_PACKAGE               = 0x11,         ///< Protocol：3.9 
    ACK_ERROR_PACKAGE               = 0x12,         ///< Protocol：3.9 

    CMD_PRODUCTION_TEST             = 0x13,         ///< Protocol：
    ACK_PRODUCTION_TEST             = 0x14,         ///< Protocol：

    CMD_NINABLE_MODE                = 0x15,         ///< Protocol：3.10
    ACK_NINABLE_MODE                = 0x16,         ///< Protocol：3.10

    CMD_GET_NTP                     = 0x17,         ///< Protocol：4.3 
    ACK_GET_NTP                     = 0x18,         ///< Protocol：4.3 


    CMD_ASK_BIGDATA                 = 0x19,         ///< Protocol：4.4
    ACK_ASK_BIGDATA                 = 0x1A,         ///< Protocol：4.4

    CMD_BIGDATA_READY               = 0x1B,         ///< Protocol：4.5
    ACK_BIGDATA_READY               = 0x1C,         ///< Protocol：4.5

    CMD_BIGDATA_SEND                = 0x1D,         ///< Protocol：4.6
    ACK_BIGDATA_SEND                = 0x1E,         ///< Protocol：4.6

    CMD_S_STOP_BIGDATA_SEND         = 0x1F,         ///< Protocol：4.7
    ACK_S_STOP_BIGDATA_SEND         = 0x20,         ///< Protocol：4.7

    CMD_D_STOP_BIGDATA_SEND         = 0x27,         ///< Protocol：4.8
    ACK_D_STOP_BIGDATA_SEND         = 0x28,         ///< Protocol：4.8

    CMD_ASK_MODULE_INFO             = 0x21,         ///< Protocol：4.9
    ACK_ASK_MODULE_INFO             = 0x22,         ///< Protocol：4.9

    CMD_ASK_AFFAIR_HANDLE           = 0x23,         ///< Protocol：4.10
    ACK_ASK_AFFAIR_HANDLE           = 0x24,         ///< Protocol：4.10

    CMD_AFFAIR_RESULT               = 0x25,         ///< Protocol：4.10
    ACK_AFFAIR_RESULT               = 0x26,         ///< Protocol：4.10

    CMD_REBOOT_MODULE               = 0x29,         ///< Protocol：3.11
    ACK_REBOOT_MODULE               = 0x2A,         ///< Protocol：3.11

    CMD_CONNECT_M2M                 = 0x2D,         ///< Protocol：for Virtualization
    ACK_CONNECT_M2M                 = 0x2E,         ///< Protocol：for Virtualization

    CMD_CONNECT_M2M_BACK            = 0x2F,         ///< Protocol：for Virtualization
    ACK_CONNECT_M2M_BACK            = 0x30,         ///< Protocol：for Virtualization

    CMD_UPLOAD_DATA                 = 0x31,         ///< Protocol：for Virtualization
    ACK_UPLOAD_DATA                 = 0x32,         ///< Protocol：for Virtualization

    CMD_UPLOAD_DATA_BACK            = 0x33,         ///< Protocol：for Virtualization
    ACK_UPLOAD_DATA_BACK            = 0x34,         ///< Protocol：for Virtualization

    CMD_DISCONNECT_M2M              = 0x35,         ///< Protocol：for Virtualization
    ACK_DISCONNECT_M2M              = 0x36,         ///< Protocol：for Virtualization

    CMD_DISCONNECT_M2M_BACK         = 0x37,         ///< Protocol：for Virtualization
    ACK_DISCONNECT_M2M_BACK         = 0x38,         ///< Protocol：for Virtualization

    CMD_RESET_SIMULATOR             = 0x39,         ///< Protocol：for Virtualization
    ACK_RESET_SIMULATOR             = 0x3A,         ///< Protocol：for Virtualization

    CMD_RESET_SIMULATOR_BACK        = 0x3B,         ///< Protocol：for Virtualization
    ACK_RESET_SIMULATOR_BACK        = 0x3C,         ///< Protocol：for Virtualization
} PROTOCOL_CMDTYPE;                                                                                  
                        
/** Illegal message type*/  
typedef enum
{
    ERROR_ACK_SUM = 0x01,                           ///< check error
    ERROR_CMD     = 0x02,                           ///< Command code error
    ERROR_OTHER   = 0x03,                           ///< other
} errorPacketsType_t;

typedef enum
{
    EXE_SUCESS                      = 0x00,
    EXE_FAILE                       = 0x01,
} execute_result;  

#pragma pack(1)

/** User Area Device State Structure */
typedef struct {
  bool valueLED;
  bool valueIS_GetUpAlarm;
  bool valueIS_GoToBedAlarm;
  uint32_t valueFireMonitorState;
  uint32_t valuePowerMonitorState;
  uint32_t valueSetGetUpHour;
  uint32_t valueSetGetUpMinute;
  uint32_t valueSetGoToBedHour;
  uint32_t valueSetGoToBedMinute;
  uint32_t valueSetPowerMonitorVlaue;
  uint32_t valueFireMonitor;
  uint32_t valueLED_status;
  uint32_t valuePowerMonitor;
  uint32_t valueDisplayPowerMonitorVlaue;
} dataPoint_t;


/** Corresponding to the protocol "4.10 WiFi module control device" in the flag " attr_flags" */ 
typedef struct {
  uint8_t flagLED:1;
  uint8_t flagIS_GetUpAlarm:1;
  uint8_t flagIS_GoToBedAlarm:1;
  uint8_t flagFireMonitorState:1;
  uint8_t flagPowerMonitorState:1;
  uint8_t flagSetGetUpHour:1;
  uint8_t flagSetGetUpMinute:1;
  uint8_t flagSetGoToBedHour:1;
  uint8_t flagSetGoToBedMinute:1;
  uint8_t flagSetPowerMonitorVlaue:1;
} attrFlags_t;


/** Corresponding protocol "4.10 WiFi module control device" in the data value "attr_vals" */

typedef struct {
  uint8_t wBitBuf[COUNT_W_BIT];
  uint8_t valueSetGetUpHour;
  uint8_t valueSetGetUpMinute;
  uint8_t valueSetGoToBedHour;
  uint8_t valueSetGoToBedMinute;
  uint16_t valueSetPowerMonitorVlaue;
} attrVals_t;

/** The flag "attr_flags (1B)" + data value "P0 protocol area" in the corresponding protocol "4.10 WiFi module control device"attr_vals(6B)" */ 
typedef struct {
    attrFlags_t attrFlags;
    attrVals_t  attrVals;
}gizwitsIssued_t;

/** Corresponding protocol "4.9 Device MCU to the WiFi module to actively report the current state" in the device status "dev_status(11B)" */ 

typedef struct {
  uint8_t wBitBuf[COUNT_W_BIT];
  uint8_t valueSetGetUpHour;
  uint8_t valueSetGetUpMinute;
  uint8_t valueSetGoToBedHour;
  uint8_t valueSetGoToBedMinute;
  uint16_t valueSetPowerMonitorVlaue;
  uint8_t rBitBuf[COUNT_R_BIT];
  uint16_t valueDisplayPowerMonitorVlaue;
} devStatus_t; 


                
/** Event queue structure */                               
typedef struct {                           
    uint8_t num;                                    ///< Number of queue member
    uint8_t event[EVENT_TYPE_MAX];                  ///< Queue member event content
}eventInfo_t;


                            
/** wifiSignal strength structure */                       
typedef struct {                           
    uint8_t rssi;                                   ///< WIFI signal strength
}moduleStatusInfo_t;                                

/** Protocol standard header structure */
typedef struct
{
    uint8_t                 head[2];                ///< The head is 0xFFFF
    uint16_t                len;                    ///< From cmd to the end of the entire packet occupied by the number of bytes
    uint8_t                 cmd;                    ///< command
    uint8_t                 sn;                     ///< 
    uint8_t                 flags[2];               ///< flag,default is 0
} protocolHead_t;

/** 4.1 WiFi module requests the device information protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 protocolVer[8];         ///< Protocol version
    uint8_t                 p0Ver[8];               ///< p0 Protocol version
    uint8_t                 hardVer[8];             ///< Hardware version
    uint8_t                 softVer[8];             ///< Software version
    uint8_t                 productKey[32];         ///< Product key
    uint16_t                ninableTime;            ///< Binding time(second)
    uint8_t                 devAttr[8];             ///< Device attribute
    uint8_t                 productSecret[32];      ///< Product secret
    uint8_t                 sum;                    ///< checksum
} protocolDeviceInfo_t;

/** Protocol common data frame(4.2、4.4、4.6、4.9、4.10) protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 sum;                    ///< checksum
} protocolCommon_t;

/** 4.3 The device MCU informs the WiFi module of the configuration mode  protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 cfgMode;                ///< Configuration parameters
    uint8_t                 sum;                    ///< checksum
} protocolCfgMode_t;

/** 4.13 The MCU requests the network time  protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 time[7];                ///< Hardware version
    uint8_t                 ntp_time[4];            ///< Software version
    uint8_t                 sum;                    ///< checksum
} protocolUTT_t;

/** WiFi module working status*/
typedef union
{
    uint16_t                value;
    struct
    {
        uint16_t            softap:1;     
        uint16_t            station:1;    
        uint16_t            onboarding:1; 
        uint16_t            binding:1;    
        uint16_t            con_route:1;  
        uint16_t            con_m2m:1;    
        uint16_t            reserve1:2;   
        uint16_t            rssi:3;       
        uint16_t            app:1;        
        uint16_t            test:1;       
        uint16_t            reserve2:3;   
    }types; 
    
} wifiStatus_t;

/** WiFi status type :protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    wifiStatus_t            ststus;                 ///< WIFI status
    uint8_t                 sum;                    ///< checksum
} protocolWifiStatus_t;

/** Protocol common data frame(4.9) :protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 type;                   ///< Information Type
    uint8_t                 sum;                    ///< checksum
} protocolGetModuleInfo_t;

typedef struct
{
    uint8_t                 moduleType;             ///< Information Type
    uint8_t                 serialVer[8];           ///< Serial port protocol version
    uint8_t                 hardVer[8];             ///< Hardware version
    uint8_t                 softVer[8];             ///< Software version
    uint8_t                 mac[16];                ///< mac 
    uint8_t                 ip[16];                 ///< ip
    uint8_t                 devAttr[8];             ///< Device attribute
} moduleInfo_t;

/** Protocol common data frame(4.9) :protocol structure */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    moduleInfo_t            wifiModuleInfo;         ///< WIFI module information
    uint8_t                 sum;                    ///< checksum
} protocolModuleInfo_t;


/** GPRS information of base station */
typedef struct
{
    uint16_t                    LAC_ID;             ///<LAC area ID
    uint16_t                    CellID;             ///<Base station ID
    uint8_t                     RSSI;               ///<Signal strength of base station
} gprsCellInfo_t;


/** 3.19 The basic information of the GPRS communication module  */
typedef struct
{
    uint8_t                 Type;//2G/3g/4g
    uint8_t                 Pro_ver[8];//Universal serial port protocol version
    uint8_t                 Hard_ver[8];//Hardware version
    uint8_t                 Soft_ver[8];//Software version
    uint8_t                 Device_attribute[8];//Device attribute
    uint8_t                 IMEI[16];//string
    uint8_t                 IMSI[16];//string
    uint8_t                 MCC[8];//Mobile country code
    uint8_t                 MNC[8];//Mobile network code
    uint8_t                 CellNum;//Number of base station
    uint8_t                 CellInfoLen;//Information length of base station 
    gprsCellInfo_t          GPRS_CellINFO[CELLNUMMAX];
}gprsInfo_t;

/** 4.7 Illegal message notification :protocol structure*/
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 error;                  ///< error value
    uint8_t                 sum;                    ///< checksum
} protocolErrorType_t;


/** P0 message header */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol standard header structure
    uint8_t                 action;                 ///< p0 command 
} protocolP0Head_t;


/** protocol “4.9 The device MCU reports the current status to the WiFi module” device status "dev_status(11B)"  */ 
typedef struct {
                     
    devStatus_t devStatus;                          ///< Stores the device status data
}gizwitsReport_t;

/** resend strategy structure */
typedef struct {
    uint8_t                 num;                    ///< resend times
    uint8_t                 flag;                   ///< 1,Indicates that there is a need to wait for the ACK;0,Indicates that there is no need to wait for the ACK
    uint8_t                 buf[MAX_PACKAGE_LEN];   ///< resend data buffer
    uint16_t                dataLen;                ///< resend data length
    uint32_t                sendTime;               ///< resend time
} protocolWaitAck_t;
                                                                                
/** 4.8 WiFi read device datapoint value , device ack use this struct */
typedef struct
{
    protocolHead_t          head;                   ///< Protocol head
    uint8_t                 action;                 ///< p0 action
    gizwitsReport_t         reportData;             ///< p0 data
    uint8_t                 sum;                    ///< Checksum 
} protocolReport_t;


/** Protocol main and very important struct */
typedef struct
{
    uint8_t issuedFlag;                             ///< P0 action type
    uint8_t protocolBuf[MAX_PACKAGE_LEN];           ///< Protocol data handle buffer
    uint8_t transparentBuff[MAX_PACKAGE_LEN];       ///< Transparent data storage area
    uint32_t transparentLen;                        ///< Transmission data length
    
    uint32_t sn;                                    ///< Message SN
    uint32_t timerMsCount;                          ///< Timer Count 
    protocolWaitAck_t waitAck;                      ///< Protocol wait ACK data structure
    
    eventInfo_t issuedProcessEvent;                 ///< Control events
    eventInfo_t wifiStatusEvent;                    ///< WIFI Status events
    eventInfo_t NTPEvent;                           ///< NTP events
    eventInfo_t moduleInfoEvent;                    ///< Module Info events

	gizwitsReport_t reportData;                     ///< The protocol reports data for standard product
    dataPoint_t gizCurrentDataPoint;                ///< Current device datapoints status
    dataPoint_t gizLastDataPoint;                   ///< Last device datapoints status
    moduleStatusInfo_t wifiStatusData;              ///< WIFI signal intensity
    protocolTime_t TimeNTP;                         ///< Network time information
#if MODULE_TYPE
    gprsInfo_t   gprsInfoNews;
#else  
    moduleInfo_t  wifiModuleNews;                   ///< WIFI module Info
#endif
	
        
}gizwitsProtocol_t; 

#pragma pack()

/**@name Gizwits user API interface
* @{
*/

extern uint32_t gizGetTimerCount(void);

void gizwitsInit(void);
int32_t gizwitsSetMode(uint8_t mode);
void gizwitsGetNTP(void);
int32_t gizwitsHandle(dataPoint_t *currentData);
int32_t gizwitsPassthroughData(uint8_t * gizdata, uint32_t len);
void gizwitsGetModuleInfo(void);
int32_t gizPutData(uint8_t *buf, uint32_t len);

/**@} */
#ifdef __cplusplus
}
#endif 

#endif
