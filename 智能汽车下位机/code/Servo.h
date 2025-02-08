/*
 * Servo.h
 *
 *  Created on: 2024��3��19��
 *      Author: Դ
 */

#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_

#include "zf_common_headfile.h"

#define  SERVO_PWM_MAX                  2500                        //����������PWM��180��
#define  SERVO_PWM_MIN                  500                         //���������СPWM��0��
#define  SERVO_PWM_MAX_L                1150                        //�������ת�����ֵPWM
#define  SERVO_PWM_MAX_R                1850                        //�������ת�����ֵPWM
#define  SERVO_PWM_MIDDLE               1500                        //�����ֵPWM

#define  SERVO_ANGLE_MAX                38.0f                       //���
#define SERVO_MOTOR_FREQ            (200 )
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

/**
* @brief    ������
**/
typedef struct
{
    uint16_t thresholdMiddle;                   //�����ֵPWM
    uint16_t thresholdLeft;                     //�������ת�����ֵPWM
    uint16_t thresholdRight;                    //�������ת�����ֵPWM
}ServoStruct;

extern ServoStruct servoStr;

void SERVO_Init(void);
void SERVO_SetPwmValue(signed int pwm);
void SERVO_SetPwmValueCorrect(signed int pwm);
void SERVO_AngleControl(float angle);




#endif /* CODE_SERVO_H_ */
