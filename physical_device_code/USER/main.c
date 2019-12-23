#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

//function statement
void UART_test(void);
void Gizwits_Init(void);


//������
int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	Gizwits_Init();         //Э���ʼ��
	LCD_Init();				//TFTLCD����ʼ��
	
	POINT_COLOR = RED;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
	LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
	LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
 	LCD_ShowString(30,110,200,16,16,"KEY0: SoftAP mode");
	LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
	

	//��ѯ
	while(1)
	{
		
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
			
		userHandle();//�û��ɼ�
	

	}	
	
}


//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
	usart3_init(9600);//UART-WIFI��ʼ��
	userInit();													//�豸״̬�ṹ���ʼ��
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


