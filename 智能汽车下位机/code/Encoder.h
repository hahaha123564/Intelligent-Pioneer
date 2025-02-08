/*
 * Encoder.h
 *
 *  Created on: 2024年3月18日
 *      Author: 源
 */

#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_

#include "zf_common_headfile.h"

void ENCODER_Init(void);
void ENCODER_RevSample (void);
//一阶卡尔曼滤波
float KalmanFilterEncoderLeft(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);



#endif /* CODE_ENCODER_H_ */
