#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

//function statement
void UART_test(void);


/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;	//��gizwits_product.c���Ѷ���
 
//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
//u8 wifi_sta=0;


//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
	usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));	//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}


//������
int main(void)
{		
 	int key;
	int32_t ret = 0;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	Gizwits_Init();         //Э���ʼ��
	
	printf("\r\n--------������IOT-LED����ʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY0:SoftAP����ģʽ\t KEY_UP:��λ\r\n\r\n");	 
 	
//��ѯ
while(1)
{
	userHandle();//�û��ɼ�
	
	gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
	
	key = KEY_Scan(0);
	
	if(key == KEY1_PRES){//KEY1 ����
		printf("WIFI ���� AirLink ����ģʽ\r\n");
		ret = gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link ģʽ����
		printf("ret = %d Bytes\r\n", ret);
		printf("IS OK\r\n");
	}else if(key == KEY0_PRES){// KEY0 ����
		printf("WIFI ����SoftAP ����ģʽ\r\n");
		ret = gizwitsSetMode(WIFI_SOFTAP_MODE);
		printf("ret = %d Bytes\r\n", ret);
		printf("IS OK\r\n");
	}else if(key == WKUP_PRES){//KEY_UP ����
		printf("WIFI ��λ����������������\r\n");
		ret = gizwitsSetMode(WIFI_RESET_MODE);//WIFI ��λ
		printf("ret = %d Bytes\r\n", ret);
		printf("IS OK\r\n");
	}
	delay_ms(200);
	LED1=!LED1;



}	
	
}


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


