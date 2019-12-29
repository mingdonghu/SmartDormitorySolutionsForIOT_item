#ifndef __VOICE_MY2480_16P_H
#define __VOICE_MY2480_16P_H

#include "sys.h"


#define MYIO5   PFout(5)
#define MYIO4   PFout(4)
#define MYIO3   PFout(3)
#define MYIO2   PFout(2)
#define MYIO1   PFout(1)



void MY2480_Init(void);
void voice1Play(void);
void voice2Play(void);
void voice3Play(void);
void volumeAdd(void);
void volumeSub(void);


#endif

