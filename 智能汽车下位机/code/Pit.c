/*
 * Pit.c
 *
 *  Created on: 2024年3月20日
 *      Author: 源
 */
#include "zf_common_headfile.h"


void TIM2_Init(void)
{
    pit_ms_init(CCU60_CH0, 1);
}


