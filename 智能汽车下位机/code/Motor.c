/*
 * Motor.c
 *
 *  Created on: 2024年3月20日
 *      Author: 源
 */

#include "zf_common_headfile.h"

MotorStruct motorStr;

#define DIR1                (P21_4)
#define PWM1                (ATOM0_CH3_P21_5)
#define DIR2                (P02_6)
#define PWM2                (ATOM0_CH7_P02_7)
#define DIR3                (P21_3)
#define PWM3                (ATOM1_CH0_P21_2)

bool dir = true;
int xy=0 ,yx = 0,q=0,t=0,f=0;

/**
* @brief        电机控制初始化
* @param
* @ref
* @author       Leo
* @note
**/
void MOTOR_Init(void)
{
    pwm_init(PWM1, 17000, 0);                                                   // PWM 通道初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM2, 17000, 0);                                                   // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    pwm_init(PWM3, 17000, 0);
    gpio_init(DIR1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // GPIO 初始化为输出 默认上拉输出高
    gpio_init(DIR2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             //电机模型初始化
    gpio_init(DIR3, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    motorStr.EncoderLine = 512.0f;                          //编码器线数=光栅数16*4
    motorStr.ReductionRatio = 2.7f;                         //电机减速比
    motorStr.EncoderValue = 0;
    motorStr.DiameterWheel = 0.064f;//68cm                  //轮子直径:m
    motorStr.CloseLoop = true;                              //默认闭环模式

}

/**
* @brief        电机输出PWM设置
* @param        pwm：-2000~2000
* @ref
* @author       Leo
* @note
**/

void MOTOR_SetPwmValue(signed int pwm)
{
    pwm = pwm;
    if(pwm>=0)
    {
        gpio_set_level(DIR1, GPIO_HIGH);
        gpio_set_level(DIR2, GPIO_HIGH);
        gpio_set_level(DIR3, GPIO_HIGH);
        if(pwm>MOTOR_PWM_MAX)
            pwm =MOTOR_PWM_MAX;
        pwm_set_duty(PWM1, pwm * (PWM_DUTY_MAX / 2000));
        pwm_set_duty(PWM2, pwm * (PWM_DUTY_MAX / 2000));
        pwm_set_duty(PWM3, pwm * (PWM_DUTY_MAX / 2000));
    }
    else if(pwm<0)
    {
        gpio_set_level(DIR1, GPIO_LOW);
        gpio_set_level(DIR2, GPIO_LOW);
        gpio_set_level(DIR3, GPIO_LOW);
        if(pwm<MOTOR_PWM_MIN)
            pwm=MOTOR_PWM_MIN;

        pwm = -pwm;
        pwm_set_duty(PWM1, pwm * (PWM_DUTY_MAX / 2000));
        pwm_set_duty(PWM2, pwm * (PWM_DUTY_MAX / 2000));
        pwm_set_duty(PWM3, pwm * (PWM_DUTY_MAX / 2000));
    }


}
/**
* @brief        电机闭环速控
* @param        speed：速度m/s
* @ref
* @author       Leo
* @note
**/
void MOTOR_ControlLoop(float speed)
{
    if(speed > MOTOR_SPEED_MAX)
        speed = MOTOR_SPEED_MAX;
    else if(speed < -MOTOR_SPEED_MAX)
        speed = -MOTOR_SPEED_MAX;

    pidStr.vi_Ref = (float)(speed*MOTOR_CONTROL_CYCLE / motorStr.DiameterWheel / PI * motorStr.EncoderLine * 4.0f * motorStr.ReductionRatio);

    MOTOR_SetPwmValue(PID_MoveCalculate(&pidStr));
}

/**
* @brief        电机控制线程
* @param
* @ref
* @author       Leo
* @note
**/
void MOTOR_Timer(void)
{
    motorStr.Counter++;
    if(motorStr.Counter >= 10)                              //速控:10ms
    {
        ENCODER_RevSample();                                //编码器采样
        if(icarStr.sprintEnable || usbStr.connected)        //通信连接或电机测试才开启闭环（保护+省电）
        {
            if(motorStr.CloseLoop)
            {
                q++;

                if(icarStr.SpeedSet == 1)
                {
                    xy++;

                }
                else if(icarStr.SpeedSet == 0.1)
                {
                    f=1;
                }
                if(f == 1)
                {
                    t++;
                }
                if((t>1)&(t<20))
                {
                    MOTOR_ControlLoop(0.25+t*0.025);
                }
                else if((t>20)&(t<25))
                {
                    MOTOR_ControlLoop(0);
                }
                else if((t>25)&(t<40))
                {
                    MOTOR_ControlLoop(-0.025*(t-25));
                }

                else if(t>40)
                {
                    t=0;
                    f=0;
                }
                if((xy>=50)&(t==0))
                {
                    MOTOR_ControlLoop(icarStr.SpeedSet/100);
                    yx++;
                    if(yx>=5)
                    {
                        xy =0;
                    }
                }
                else if((1440<icarStr.ServoPwmSet)&(1560>icarStr.ServoPwmSet)&(q>=10)&(t==0))
                {
                    MOTOR_ControlLoop(icarStr.SpeedSet/2);
                }
                else if((t==0))
                {
                    MOTOR_ControlLoop(icarStr.SpeedSet*7/20);
                    q=0;
                }


            }
            else//开环百分比控制
            {
                if(icarStr.SpeedSet > 100)
                    icarStr.SpeedSet = 100;
                else if(icarStr.SpeedSet < -100)
                    icarStr.SpeedSet = -100;
                signed int speedRate = MOTOR_PWM_MAX/100.f*icarStr.SpeedSet; //开环：百分比%

                MOTOR_SetPwmValue(speedRate);       //开环速控
            }
        }
        else
        {
            MOTOR_SetPwmValue(0);
        }

        motorStr.Counter = 0;
    }
}
















