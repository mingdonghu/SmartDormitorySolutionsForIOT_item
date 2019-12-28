#ifndef __POWERSENSOR_IM1281B_H
#define __POWERSENSOR_IM1281B_H	 
#include "sys.h"  



extern uint8_t read_enable;

extern float Voltage_data;
extern float Current_data;
extern float Power_data;
extern float Energy_data;
extern float Pf_data;
extern float CO2_data;


void usart2_init(u32 bound);			//����2��ʼ�� 

uint8_t calc_crc(uint8_t crc_buf, uint8_t crc);
uint32_t chk_crc(uint8_t *buf, uint8_t len);
void read_data(void);
void analysis_data(void);




#endif

