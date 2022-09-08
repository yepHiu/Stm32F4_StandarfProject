#ifndef _BEEP_H_
#define _BEEP_H_

#include "includes.h"

#define BEEP_ON GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define BEEP_OFF GPIO_SetBits(GPIOA,GPIO_Pin_7)

void InitBEEP(void);

#endif // !_BEEP_H_






