/*
 * Encoder.h
 *
 *  Created on: 2024��3��18��
 *      Author: Դ
 */

#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_

#include "zf_common_headfile.h"

void ENCODER_Init(void);
void ENCODER_RevSample (void);
//һ�׿������˲�
float KalmanFilterEncoderLeft(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);



#endif /* CODE_ENCODER_H_ */
