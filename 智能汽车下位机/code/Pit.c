/*
 * Pit.c
 *
 *  Created on: 2024��3��20��
 *      Author: Դ
 */
#include "zf_common_headfile.h"


void TIM2_Init(void)
{
    pit_ms_init(CCU60_CH0, 1);
}


