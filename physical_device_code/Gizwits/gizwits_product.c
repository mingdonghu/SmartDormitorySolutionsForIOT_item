/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "common.h"

#include "usart3.h"		//添加与修改
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "delay.h"


static uint32_t timerMsCount;

/** Current datapoint */

dataPoint_t currentDataPoint;

uint8_t RxWifiBuffer = 0;


uint8_t wifi_state= 0;	//记录WiFi连接状态， wifi_state: 0 -- wifi断开; wifi_state : 1 -- wifi连接

protocolTime_t GetNtpVal = {0,0,0,0,0,0,0}; //记录网络同步时间[年-月-日-时-分-秒]，24小时制

uint8_t key_status;


/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_LED:
        currentDataPoint.valueLED = dataPointPtr->valueLED;
        GIZWITS_LOG("Evt: EVENT_LED %d \n", currentDataPoint.valueLED);
        if(0x01 == currentDataPoint.valueLED)
        {
          //user handle
          LED0 = 0; //LED0 亮
        }
        else
        {
          //user handle
          LED0 = 1; //LED0 灭
        }
        break;




      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
	  		wifi_state = 1; //wifi已连接
			GIZWITS_LOG("\r\n wifi is connect! \r\n");
				LCD_Clear(BLUE);
				LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
				LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
				LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
				LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
				LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
				LCD_ShowString(50,170,200,16,16,"WIFI is connect!");
        break;
      case WIFI_DISCON_M2M:	
	  		wifi_state = 0; //wifi已断开
	  		GIZWITS_LOG("\r\n wifi is disconnect! \r\n");
				LCD_Clear(RED);
				LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
				LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
				LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
				LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
				LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
				LCD_ShowString(50,170,200,16,16,"WIFI is disconnect!");
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
				GetNtpVal.year = ptime->year;
				GetNtpVal.month = ptime->month;
				GetNtpVal.day   = ptime->day;
				GetNtpVal.hour  = ptime->hour;
				GetNtpVal.minute = ptime->minute;
				GetNtpVal.second = ptime->second;
			if(key_status== KEY1_PRES)
			{
				LCD_Clear(BLUE);
				LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
				LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
				LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
				LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
				LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
				LCD_ShowString(50,170,200,16,16,"WIFI is connect!");
				
				LCD_ShowNum(30,150,GetNtpVal.year,4,16);
				LCD_ShowChar(70,150,'-',12,1);
				LCD_ShowNum(90,150,GetNtpVal.month,2,16);
				LCD_ShowChar(110,150,'-',12,1);
				LCD_ShowNum(130,150,GetNtpVal.day,2,16);
				LCD_ShowChar(150,150,'#',12,1);
				LCD_ShowNum(170,150,GetNtpVal.hour,2,16);
				LCD_ShowChar(190,150,':',12,1);
				LCD_ShowNum(210,150,GetNtpVal.minute,2,16);
				LCD_ShowChar(230,150,':',12,1);
				LCD_ShowNum(250,150,GetNtpVal.second,2,16);
			}
		break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/


void userHandle(void)
{
	static u8 t = 0;

	if(wifi_state){
		t++;
		if(t == 10){
			t = 0;
			gizwitsGetNTP(); //请求NTP网络时间
		}
		delay_ms(100);
	}else{
		if(t != 0){
			t = 0;
		}
	}
	
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueLED = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
	__set_FAULTMASK(1);//关闭所有中断
  NVIC_SystemReset();//复位
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
	int key = 0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		key = KEY_Scan(0);
			if(key == KEY1_PRES)
			{//KEY1 按键
				key_status = key;
				gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link 模式接入
				LCD_Clear(WHITE);
				LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
				LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
				LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
				LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
				LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
				LCD_ShowString(50,170,200,16,16,"WIFI is ok in Airlink");
			}
			else if(key == KEY0_PRES)
			{// KEY0 按键
				key_status = key;
				//gizwitsSetMode(WIFI_SOFTAP_MODE);
				LCD_Clear(WHITE);
				LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
				LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
				LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
				LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
				LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
				LCD_ShowString(50,170,200,16,16,"WIFI is ok in SoftAP");
			}
			else if(key == WKUP_PRES)
			{//KEY_UP 按键
				key_status = key;
				gizwitsSetMode(WIFI_RESET_MODE);//WIFI 复位
				LCD_Clear(WHITE);
				LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
				LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
				LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
				LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
				LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
				LCD_ShowString(50,170,200,16,16,"WIFI is ok in RESET");
			}
		
		gizTimerMs();
	
	}

}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function 锛孎or Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  
  //value = USART_ReceiveData(USART2);//STM32 test demo
  

  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		
		RxWifiBuffer =USART_ReceiveData(USART3);
		
		gizPutData((uint8_t *)&RxWifiBuffer, 1);//数据写入到缓冲区
		
		//GIZWITS_LOG("\r\n usart3_RX_IT is ok ! \r\n");
	}
	
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1锛孖nput Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    for(i=0; i<len; i++)
    {
        //USART_SendData(UART, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
        
			USART_SendData(USART3,buf[i]);
	    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
	        
			if(i >=2 && buf[i] == 0xFF)
			{
				//Serial port to achieve the function, the 0x55 sent to the module
				//USART_SendData(UART, 0x55);//STM32 test demo
						
				USART_SendData(USART3,0x55);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕 
			}
			
    }
		
		#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("\r\n MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
			if(i >=2 && buf[i] == 0xFF)
			{
				GIZWITS_LOG("%02x ", 0x55);	
			}
    }
    GIZWITS_LOG("\r\n");
    #endif

    return len;
}


