#include "PowerSensor_IM1281B.h"
   
#define Read_ID 0x01     //IM1281B 模块默认ID为0x01

union crc_data{
		uint32_t word16;
		uint8_t byte[2];
};


uint8_t Tx_Buffer[8] = {0};
uint8_t Rx_Buffer[29]= {0};
uint8_t read_enable = 0;
uint8_t receive_finished = 0;
uint8_t receive_number = 0;

float Voltage_data = 0;
float Current_data = 0;
float Power_data = 0;
float Energy_data = 0;
float Pf_data = 0;
float CO2_data = 0;


uint8_t calc_crc(uint8_t crc_buf, uint8_t crc)
{
	uint8_t i;
	uint8_t chk;

	for(i = 0; i < 8; i++)
	{
		chk = (uint8_t)(crc&1);
		crc = crc >> 1;
		crc = crc & 0x7fff;
		if(chk == 1){
			crc = crc ^ 0xa001;
			crc = crc & 0xffff;
		}
	}
	return crc;
	
}

uint32_t chk_crc(uint8_t *buf, uint8_t len)
{
	uint8_t hi,lo;
	uint32_t i;
	uint32_t crc;

	crc = 0xffff;
	for(i = 0; i < len; i++){
		crc = calc_crc(*buf, crc);
		buf++;
	}
	hi = (uint8_t)(crc % 256);
	lo = (uint8_t)(crc / 256);
	crc = (((uint32_t)(hi)) << 8 ) | lo;

	return crc;
}

void read_data(void)
{
	uint8_t i;
	union crc_data crc_now = {0, 0};

	if(read_enable == 1){
		//一秒钟抄读一次模块
		read_enable = 0; //清除1秒钟到读标志
		Tx_Buffer[0] = Read_ID;
		Tx_Buffer[1] = 0x03;
		Tx_Buffer[2] = 0x00;
		Tx_Buffer[3] = 0x48;
		Tx_Buffer[4] = 0x00;
		Tx_Buffer[5] = 0x06;
		crc_now.word16 = chk_crc(Tx_Buffer,6);
		Tx_Buffer[6] = crc_now.byte[1];  //CRC校验 低字节码在前
 		Tx_Buffer[7] = crc_now.byte[0];
		//向模块发送8Bytes data
		for(i = 0; i < sizeof(Tx_Buffer)/sizeof(Tx_Buffer[0]); i++){
			USART_SendData(USART2,Tx_Buffer[i]);
	    	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
		}
			
	}
}

void analysis_data(void)
{
	uint8_t i;
	uint32_t eeData[6] = {0};
	union crc_data crc_now = {0, 0};

	if(receive_finished == 1){ //接收完成

		receive_finished = 0; //清除接收完成标志
		if(Rx_Buffer[0] = Read_ID){	//确认ID正确

			crc_now.word16 = chk_crc(Rx_Buffer,receive_number - 2); //receive_number是接收数据总长度

			if((crc_now.byte[0] == Rx_Buffer[receive_number - 1]) && 
				(crc_now.byte[1] == Rx_Buffer[receive_number - 2])){//确定CRC校验正确

				eeData[0] = (((uint32_t)(Rx_Buffer[3])) << 24 )| (((uint32_t)(Rx_Buffer[4])) << 16 ) |
					(((uint32_t)(Rx_Buffer[5])) << 8 ) | Rx_Buffer[6];
				Voltage_data = eeData[0] * 0.0001 ; //V
				
				eeData[1] = (((uint32_t)(Rx_Buffer[7])) << 24 )| (((uint32_t)(Rx_Buffer[8])) << 16 ) |
					(((uint32_t)(Rx_Buffer[9])) << 8 ) | Rx_Buffer[10];
				Current_data = eeData[1] * 0.0001; //A
				
				eeData[2] = (((uint32_t)(Rx_Buffer[11])) << 24 )| (((uint32_t)(Rx_Buffer[12])) << 16 ) |
									(((uint32_t)(Rx_Buffer[13])) << 8 ) | Rx_Buffer[14];
				Power_data = eeData[2] * 0.0001; //W
				
				eeData[3] = (((uint32_t)(Rx_Buffer[15])) << 24 )| (((uint32_t)(Rx_Buffer[16])) << 16 ) |
									(((uint32_t)(Rx_Buffer[17])) << 8 ) | Rx_Buffer[18];
				Energy_data = eeData[3] * 0.0001; //KWh
				
				eeData[4] = (((uint32_t)(Rx_Buffer[19])) << 24 )| (((uint32_t)(Rx_Buffer[20])) << 16 ) |
									(((uint32_t)(Rx_Buffer[21])) << 8 ) | Rx_Buffer[22];
				Pf_data = eeData[4] * 0.001; //功率因数 P/Q
				
				eeData[5] = (((uint32_t)(Rx_Buffer[23])) << 24 )| (((uint32_t)(Rx_Buffer[24])) << 16 ) |
									(((uint32_t)(Rx_Buffer[25])) << 8 ) | Rx_Buffer[26];
				CO2_data = eeData[5] * 0.0001; //kg
				
			}
		}
	}
}



//串口2中断服务函数
void USART2_IRQHandler(void)
{
	static uint8_t count = 0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){	//接收到数据
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);

		if(count >= 29){
			count = 0;
		}
		else{
			Rx_Buffer[count] = USART_ReceiveData(USART2);
			if(count == 28){
				receive_finished= 1; //接收完成
				receive_number = 29;
			}
			count++;
		}
		
	}

}   

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	     与IM1281B单相电能计量传感器通讯，默认为4800bps
void usart2_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	                       //GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);                          //串口2时钟使能

 	USART_DeInit(USART2);  //复位串口2
	//USART2_TX   PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                     //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                               //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA2
   
    //USART2_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA3
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率一般设置为4800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式
  
	USART_Init(USART2, &USART_InitStructure);       //初始化串口2
  
	USART_Cmd(USART2, ENABLE);                      //使能串口 
	
	//使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

