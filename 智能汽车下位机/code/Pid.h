/*
 * Pid.h
 *
 *  Created on: 2024��3��20��
 *      Author: Դ
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#include "zf_common_headfile.h"

#define VV_DEADLINE             2                       //�ٶ�PID������������Χ����������ֹǿӲ�̶�
#define PID_VKP                 4.0f                    //PID�궨ֵ    1.6f
#define PID_VKI                 0.1f                    //PID�궨ֵ    0.5f
#define PID_VKD                 0.03f                   //PID�궨ֵ   0.007f

/**
* @brief    PID���
**/
typedef struct
{
    float vi_Ref;                       //�ٶ�PID���ٶ��趨ֵ
    float vi_FeedBack;                  //�ٶ�PID���ٶȷ���ֵ
    float vi_PreError;                  //�ٶ�PID���ٶ����,vi_Ref - vi_FeedBack
    float vi_PreDerror;                 //�ٶ�PID��ǰһ�Σ��ٶ����֮�d_error-PreDerror;
    float v_Kp;                         //����ϵ����Kp = Kp
    float v_Ki;                         //����ϵ����Ki = Kp * ( T / Ti )
    float v_Kd;                         //΢��ϵ����Kd = KP * Td * T
    float vl_PreU;                      //PID���ֵ
}PIDStruct;

extern PIDStruct pidStr;

void PID_Init(void);
signed int PID_MoveCalculate(PIDStruct *pp);






#endif /* CODE_PID_H_ */
