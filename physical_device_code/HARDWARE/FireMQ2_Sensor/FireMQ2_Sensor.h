#ifndef __FIREMQ2_SENSOR_H
#define __FIREMQ2_SENSOR_H

#include "sys.h"

//#define readMQ_DO PFin(0)

#define readMQ_DO GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)


void EXTIX_Init(void);


#endif

