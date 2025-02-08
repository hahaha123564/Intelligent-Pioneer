/*
 * Servo.h
 *
 *  Created on: 2024年3月19日
 *      Author: 源
 */

#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_

#include "zf_common_headfile.h"

#define  SERVO_PWM_MAX                  2500                        //舵机方向最大PWM：180°
#define  SERVO_PWM_MIN                  500                         //舵机方向最小PWM：0°
#define  SERVO_PWM_MAX_L                1150                        //舵机左向转角最大值PWM
#define  SERVO_PWM_MAX_R                1850                        //舵机右向转角最大值PWM
#define  SERVO_PWM_MIDDLE               1500                        //舵机中值PWM

#define  SERVO_ANGLE_MAX                38.0f                       //舵机
#define SERVO_MOTOR_FREQ            (200 )
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

/**
* @brief    舵机相关
**/
typedef struct
{
    uint16_t thresholdMiddle;                   //舵机中值PWM
    uint16_t thresholdLeft;                     //舵机左向转角最大值PWM
    uint16_t thresholdRight;                    //舵机右向转角最大值PWM
}ServoStruct;

extern ServoStruct servoStr;

void SERVO_Init(void);
void SERVO_SetPwmValue(signed int pwm);
void SERVO_SetPwmValueCorrect(signed int pwm);
void SERVO_AngleControl(float angle);




#endif /* CODE_SERVO_H_ */
