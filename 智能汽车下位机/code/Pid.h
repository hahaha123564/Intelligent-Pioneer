/*
 * Pid.h
 *
 *  Created on: 2024年3月20日
 *      Author: 源
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#include "zf_common_headfile.h"

#define VV_DEADLINE             2                       //速度PID，设置死区范围，消抖，静止强硬程度
#define PID_VKP                 4.0f                    //PID标定值    1.6f
#define PID_VKI                 0.1f                    //PID标定值    0.5f
#define PID_VKD                 0.03f                   //PID标定值   0.007f

/**
* @brief    PID相关
**/
typedef struct
{
    float vi_Ref;                       //速度PID，速度设定值
    float vi_FeedBack;                  //速度PID，速度反馈值
    float vi_PreError;                  //速度PID，速度误差,vi_Ref - vi_FeedBack
    float vi_PreDerror;                 //速度PID，前一次，速度误差之差，d_error-PreDerror;
    float v_Kp;                         //比例系数，Kp = Kp
    float v_Ki;                         //积分系数，Ki = Kp * ( T / Ti )
    float v_Kd;                         //微分系数，Kd = KP * Td * T
    float vl_PreU;                      //PID输出值
}PIDStruct;

extern PIDStruct pidStr;

void PID_Init(void);
signed int PID_MoveCalculate(PIDStruct *pp);






#endif /* CODE_PID_H_ */
