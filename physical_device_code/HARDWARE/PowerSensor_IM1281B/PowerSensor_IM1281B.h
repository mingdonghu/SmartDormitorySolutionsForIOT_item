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


void usart2_init(u32 bound);			//´®¿Ú2³õÊ¼»¯ 

uint32_t calc_crc(uint8_t crc_buf, uint32_t crc);
uint32_t chk_crc(uint8_t *buf, uint8_t len);
void read_IM1281B_data(void);
void analysis_IM1281B_data(void);




#endif

