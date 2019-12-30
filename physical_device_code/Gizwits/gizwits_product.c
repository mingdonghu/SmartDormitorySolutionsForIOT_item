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
#include "PowerSensor_IM1281B.h"
#include "Voice_MY2480_16p.h"
#include "FireMQ2_Sensor.h"


static uint32_t timerMsCount;

/** Current datapoint */

dataPoint_t currentDataPoint;

uint8_t RxWifiBuffer = 0;


uint8_t wifi_state= 0;	//记录WiFi连接状态， wifi_state: 0 -- wifi断开; wifi_state : 1 -- wifi连接

protocolTime_t GetNtpVal = {0,0,0,0,0,0,0}; //记录网络同步时间[年-月-日-时-分-秒]，24小时制

uint8_t key_status;

uint8_t led_status = 1; //1:初始状态为关灯

uint8_t fire_status = 0;

uint32_t power_cmp_val = 1000; //W 大功率设备考量值

uint8_t  EN_GetUpAlarm = 0; //1:使能起床闹钟
uint8_t  EN_GoToBedAlarm = 0; //1:使能熄灯闹钟
uint8_t  GetUpHourSetVal = 0; //起床闹钟时钟设置值
uint8_t  GetUpMinuteSetVal = 0; //起床闹钟分钟设置值
uint8_t  GoToBedHourSetVal = 0;	//熄灯闹钟时钟设置值
uint8_t  GoToBedMinuteSetVal = 0;//熄灯闹钟分钟设置值

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
          led_status = 0;
        }
        else
        {
          //user handle
          LED0 = 1; //LED0 灭
          led_status = 1;
        }
        break;
				
	 case EVENT_IS_GetUpAlarm:
        currentDataPoint.valueIS_GetUpAlarm = dataPointPtr->valueIS_GetUpAlarm;
        GIZWITS_LOG("Evt: EVENT_IS_GetUpAlarm %d \n", currentDataPoint.valueIS_GetUpAlarm);
        if(0x01 == currentDataPoint.valueIS_GetUpAlarm)
        {
          //user handle
          EN_GetUpAlarm = 1; //开启起床闹钟
        }
        else
        {
          //user handle 
          EN_GetUpAlarm = 0;  //关闭起床闹钟
        }
        break;
				
      case EVENT_IS_GoToBedAlarm:
        currentDataPoint.valueIS_GoToBedAlarm = dataPointPtr->valueIS_GoToBedAlarm;
        GIZWITS_LOG("Evt: EVENT_IS_GoToBedAlarm %d \n", currentDataPoint.valueIS_GoToBedAlarm);
        if(0x01 == currentDataPoint.valueIS_GoToBedAlarm)
        {
          //user handle
          EN_GoToBedAlarm = 1; //开启熄灯闹钟
        }
        else
        {
          //user handle    
          EN_GoToBedAlarm = 0; //关闭熄灯闹钟
        }
        break;
				
	   case EVENT_SetGetUpHour:
        currentDataPoint.valueSetGetUpHour = dataPointPtr->valueSetGetUpHour;
        GIZWITS_LOG("Evt:EVENT_SetGetUpHour %d\n",currentDataPoint.valueSetGetUpHour);
        //user handle
        GetUpHourSetVal = currentDataPoint.valueSetGetUpHour; //记录起床闹钟的时钟设置
        break;
			
      case EVENT_SetGetUpMinute:
        currentDataPoint.valueSetGetUpMinute = dataPointPtr->valueSetGetUpMinute;
        GIZWITS_LOG("Evt:EVENT_SetGetUpMinute %d\n",currentDataPoint.valueSetGetUpMinute);
        //user handle
        GetUpMinuteSetVal = currentDataPoint.valueSetGetUpMinute; //记录起床闹钟的分钟设置
        break;
			
      case EVENT_SetGoToBedHour:
        currentDataPoint.valueSetGoToBedHour = dataPointPtr->valueSetGoToBedHour;
        GIZWITS_LOG("Evt:EVENT_SetGoToBedHour %d\n",currentDataPoint.valueSetGoToBedHour);
        //user handle
        GoToBedHourSetVal = currentDataPoint.valueSetGoToBedHour; //记录熄灯闹钟的时钟设置
        break;
			
	 case EVENT_SetGoToBedMinute:
        currentDataPoint.valueSetGoToBedMinute = dataPointPtr->valueSetGoToBedMinute;
        GIZWITS_LOG("Evt:EVENT_SetGoToBedMinute %d\n",currentDataPoint.valueSetGoToBedMinute);
        //user handle
        GoToBedMinuteSetVal = currentDataPoint.valueSetGoToBedMinute; //记录熄灯闹钟的时钟设置
        break;
		
	 case EVENT_FireMonitorState:       
		currentDataPoint.valueFireMonitorState = dataPointPtr->valueFireMonitorState;     
		GIZWITS_LOG("Evt: EVENT_FireMonitorState %d\n", currentDataPoint.valueFireMonitorState);   
		switch(currentDataPoint.valueFireMonitorState)
		{         
			case FireMonitorState_VALUE0:           
				//user handle
				//工作模式
				break;         
			case FireMonitorState_VALUE1:           
				//user handle
				//复位模式
				fire_status = 0; //解除发生火灾
				break;          
			default:           
				break;        
		}       
		break;

	 case EVENT_PowerMonitorState:
		currentDataPoint.valuePowerMonitorState = dataPointPtr->valuePowerMonitorState;
		GIZWITS_LOG("Evt: EVENT_PowerMonitorState %d\n", currentDataPoint.valuePowerMonitorState);
		switch(currentDataPoint.valuePowerMonitorState)
		{
			case PowerMonitorState_VALUE0:
				//user handle
				//工作模式
				break;
			case PowerMonitorState_VALUE1:
				//user handle
				//复位模式
				currentDataPoint.valuePowerMonitor = PowerMonitor_VALUE0; //不存在大功率
				break;
			default:
				break;
		}
		break;

	  case EVENT_SetPowerMonitorVlaue:
			currentDataPoint.valueSetPowerMonitorVlaue = dataPointPtr->valueSetPowerMonitorVlaue;
			GIZWITS_LOG("Evt:EVENT_SetPowerMonitorVlaue %d\n",currentDataPoint.valueSetPowerMonitorVlaue);
			//user handle
			power_cmp_val = currentDataPoint.valueSetPowerMonitorVlaue;
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
	
	if(wifi_state){		//wifi模组已连接云端

		read_IM1281B_data();   //读取IM1281B单相电能计量模块的数据
		analysis_IM1281B_data();//解析IM1281B单相电能计量模块发送至MCU的数据

		if(fire_status == 1){
			currentDataPoint.valueFireMonitor = FireMonitor_VALUE0;//上报有火灾
		}
		else{
			currentDataPoint.valueFireMonitor = FireMonitor_VALUE1; //上报无火灾
		}

		if(Power_data > power_cmp_val){
			currentDataPoint.valuePowerMonitor = PowerMonitor_VALUE1; //存在大功率用电器设备
		}

		currentDataPoint.valueDisplayPowerMonitorVlaue = power_cmp_val;//上报大功率监测的下限值【比较值】
		
		switch(led_status){
			case 0: currentDataPoint.valueLED_status = LED_status_VALUE0; //上报灯光开状态
					break;
			case 1: currentDataPoint.valueLED_status = LED_status_VALUE1; //上报灯光关状态
					break;
			default: break;
		}

		t++;
		if(t == 10){
			t = 0;
			gizwitsGetNTP(); //请求NTP网络时间
		}
		delay_ms(100);
		
	}
	else{
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
	static uint32_t countRead = 0; 
	static uint32_t countMQ = 0;
	u8 GetUpFlag = 0;
	u8 GoToBedFlag = 0;
	static uint32_t countGetUp = 0;
	static uint32_t countGoToBed = 0;
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 

		if(countRead >= 999){ //1S产生一次读IM1281B电能计量模块的标志
			read_enable = 1;
			countRead = 0;
			volumeAdd();  //增加扬声器音量
		}else{
			countRead++;
		}
	
		if(readMQ_DO == 0){
			//user_Handel
			if(countMQ>= 999){ //1秒钟一次火灾警报
				voice2Play();
				fire_status = 1; //发生火灾
				countMQ = 0;
			}else{
				countMQ++;
			}
			
		}
		
		if(EN_GetUpAlarm == 1){ //设置了起床闹钟
			if(GetNtpVal.hour == GetUpHourSetVal && GetNtpVal.minute == GetUpMinuteSetVal){
				if(countGetUp >= 999){ //1秒钟
					GetUpFlag = 1;
				}else{
					countGetUp++;
				}
			}
		}
		
		if(EN_GoToBedAlarm == 1){//设置了熄灯闹钟
			if(GetNtpVal.hour == GoToBedHourSetVal && GetNtpVal.minute == GoToBedMinuteSetVal){
				if(countGoToBed >= 999){ //1秒钟
					GoToBedFlag = 1;
				}else{
					countGoToBed++;
				}
			}
		}
		
		if(GetUpFlag == 1 ){
			voice1Play(); //播放起床号
		}

		if(GoToBedFlag == 1){
			voice3Play();	//播放熄灯号
			delay_ms(1000);
			LED0 = 1;  //熄灯
			led_status = 1;
		}
		
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
			LCD_Clear(RED);
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


