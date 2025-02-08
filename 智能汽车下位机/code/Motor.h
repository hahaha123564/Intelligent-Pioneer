/*
 * Motor.h
 *
 *  Created on: 2024��3��20��
 *      Author: Դ
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#include "zf_common_headfile.h"

#define  MOTOR_PWM_MAX              1500        //OCR=95%,��ֹ��ռ�ձ���������MOS��
#define  MOTOR_PWM_MIN              -1500       //OCR=95%
#define  MOTOR_SPEED_MAX            10.0f       //������ת��(m/s) (0.017,8.04)
#define  MOTOR_CONTROL_CYCLE        0.01f       //�����������T��10ms

/**
* @brief    ������
**/
typedef struct
{
    float ReductionRatio ;                      //������ٱ�
    float EncoderLine ;                         //����������=��դ��16*4
    signed int EncoderValue;                    //������ʵʱ�ٶ�
    float DiameterWheel;                        //����ֱ����mm
    bool CloseLoop;                             //����ģʽ
    uint16_t Counter;                           //�̼߳�����
}MotorStruct;


extern MotorStruct motorStr;


void MOTOR_Init(void);
void MOTOR_SetPwmValue(signed int pwm);
void MOTOR_ControlLoop(float speed);
void MOTOR_Timer(void);




#endif /* CODE_MOTOR_H_ */
