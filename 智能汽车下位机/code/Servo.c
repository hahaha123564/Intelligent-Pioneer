/*
 * Servo.c
 *
 *  Created on: 2024年3月19日
 *      Author: 源
 */
#include "zf_common_headfile.h"

ServoStruct servoStr;
int m=0,h=0;
void SERVO_Init(void)
{
    pwm_init(ATOM1_CH1_P33_9, 200, 8800);
    SERVO_SetPwmValue(servoStr.thresholdMiddle);
}



/**
* @brief        舵机输出PWM设置
* @param        pwm：-20000~20000
* @ref
* @author       Leo
* @note
**/
void SERVO_SetPwmValue(signed int pwm)
{
    int Servo;

    if(pwm < SERVO_PWM_MIN)
        pwm = SERVO_PWM_MIN;
    else if(pwm > SERVO_PWM_MAX)
        pwm = SERVO_PWM_MAX;
    if(icarStr.SpeedSet == 0.1)
    {
        pwm =1900;
    }
    Servo= ((((pwm-500)*180/2000)-90)*1.2)+90;                //错误

    if((Servo<100)&(Servo>95))
    {
        Servo=((((pwm-500)*180/2000)-90)*(Servo-90)/10)+90;
    }
    else if((Servo<=95)&(Servo>=90))
    {
        Servo=((((pwm-500)*180/2000)-90)*0.5)+90;
    }
    else if((Servo<85)&(Servo>80))
    {
        Servo=((((pwm-500)*180/2000)-90)*(90-Servo)/10)+90;
    }
    else if((Servo<=90)&(Servo>=85))
    {
        Servo=((((pwm-500)*180/2000)-90)*0.5)+90;
    }

    pwm_set_duty(ATOM1_CH1_P33_9,SERVO_MOTOR_DUTY(Servo));
}

/**
* @brief        舵机输出PWM设置（矫正后）
* @param        pwm：500~2500
* @ref
* @author       Leo
* @note
**/
uint16_t pwm_Servo = 0;
void SERVO_SetPwmValueCorrect(signed int pwm)
{
    int Servo;
    int m;
    pwm = 3000 - pwm;  //左→右

    pwm -= servoStr.thresholdMiddle-SERVO_PWM_MIDDLE; //中值补偿

    uint16_t pwmMax = 3000 - servoStr.thresholdLeft;
    uint16_t pwmMin = 3000 - servoStr.thresholdRight;
    if(icarStr.SpeedSet == 0.1)
    {
         pwm =2300;
         h=1;
    }


    if(pwm < pwmMin)
        pwm = pwmMin;
    else if(pwm > pwmMax)
        pwm = pwmMax;

    pwm_Servo = pwm;
    Servo=((pwm-500)*180/2000);
    /*if((Servo<100)&(Servo>95))
    {
        Servo=((((pwm-500)*180/2000)-90)*(Servo-90)/10)+90;
    }
    else if((Servo<=95)&(Servo>=90))
    {
        Servo=((((pwm-500)*180/2000)-90)*0.5)+90;
    }
    else if((Servo<85)&(Servo>80))
    {
        Servo=((((pwm-500)*180/2000)-90)*(90-Servo)/10)+90;
    }
    else if((Servo<=90)&(Servo>=85))
    {
        Servo=((((pwm-500)*180/2000)-90)*0.5)+90;
    }*/

    pwm_set_duty(ATOM1_CH1_P33_9,SERVO_MOTOR_DUTY(Servo));
}
/**
* @brief       舵机角度控制
* @param
* @ref
* @author
* @note
**/
uint16_t ServoPwm = 1500;
void SERVO_AngleControl(float angle)
{
    servoStr.thresholdMiddle=1500;
    uint16_t pwm = 1500;
    angle = -angle;
    if(angle > SERVO_ANGLE_MAX)
        angle = SERVO_ANGLE_MAX;
    else if(angle < -SERVO_ANGLE_MAX)
        angle = -SERVO_ANGLE_MAX;

    if(angle >= 0)  //右转
        pwm = (float)angle/SERVO_ANGLE_MAX * (SERVO_PWM_MAX_R-servoStr.thresholdMiddle) + servoStr.thresholdMiddle;     //绝对角度计算
    else if(angle < 0)      //左转
        pwm = (float)angle/SERVO_ANGLE_MAX * (servoStr.thresholdMiddle - SERVO_PWM_MAX_L) + servoStr.thresholdMiddle;       //绝对角度计算
    ServoPwm = pwm;
    SERVO_SetPwmValue(pwm);
}



