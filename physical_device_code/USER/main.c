#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 
#include "Voice_MY2480_16p.h"
#include "PowerSensor_IM1281B.h"
#include "FireMQ2_Sensor.h"


//function statement
void UART_test(void);
void Gizwits_Init(void);


//������
int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //��־��ӡ���Դ��ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	Gizwits_Init();         //��ʼ�������ƴ���-WiFiЭ��Ӳ���ӿ�
	LCD_Init();				//TFTLCD����ʼ��
	MY2480_Init();    //��������ģ���ʼ��
	usart2_init(4800);   //��IM1281B������ܼ���������ͨѶ��Ĭ��Ϊ4800bps,USART2_TX   PA2   USART2_RX   PA3
	EXTIX_Init();       //MQ-2������ȼ���崫������Ӧ��ʼ�������ڻ��ּ�⡿
	
	
	POINT_COLOR = BLACK;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
	LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
	LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
 	LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
	LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
	
	//��ѯ
	while(1){
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
			
		userHandle();//�û��ɼ��ϴ�
	}	
	
}


//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
	usart3_init(9600);//UART-WIFI��ʼ��
	userInit();		//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}



void UART_test(void)
{
	u16 t;  
	u16 len;

	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		printf("\r\n�����͵���ϢΪ:\r\n\r\n");
		for(t=0;t<len;t++)
		{
			USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		printf("\r\n\r\n");//���뻻��
		USART_RX_STA=0;	
	}
	else
	{
		printf("����������,�Իس�������\n");
		LED1=!LED1;//��˸LED1[green],��ʾϵͳ��������.
		delay_ms(100); 	
	}
}


