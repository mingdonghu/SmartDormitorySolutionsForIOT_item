#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

 
void UART_test(void)
{
	u16 t;  
	u16 len;

	if(USART_RX_STA&0x8000){					   
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		printf("\r\n�����͵���ϢΪ:\r\n\r\n");
		for(t=0;t<len;t++){
			USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		printf("\r\n\r\n");//���뻻��
		USART_RX_STA=0;
			
	}else{
		printf("����������,�Իس�������\n");
		LED1=!LED1;//��˸LED1[green],��ʾϵͳ��������.
		delay_ms(100); 	
	}
}


int main(void)
{		
 		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
 	
//��ѯ
while(1)
{
		//UART_test();

	LED0 = !LED0;
	delay_ms(300);
	LED1 = !LED1;
	delay_ms(300);
		
}	
	
}

