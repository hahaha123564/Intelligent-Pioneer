/*
 * Motor.c
 *
 *  Created on: 2024��3��20��
 *      Author: Դ
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
* @brief        ������Ƴ�ʼ��
* @param
* @ref
* @author       Leo
* @note
**/
void MOTOR_Init(void)
{
    pwm_init(PWM1, 17000, 0);                                                   // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
    pwm_init(PWM2, 17000, 0);                                                   // ��ʼ�� PWM ͨ�� Ƶ�� 17KHz ��ʼռ�ձ� 0%
    pwm_init(PWM3, 17000, 0);
    gpio_init(DIR1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // GPIO ��ʼ��Ϊ��� Ĭ�����������
    gpio_init(DIR2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             //���ģ�ͳ�ʼ��
    gpio_init(DIR3, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    motorStr.EncoderLine = 512.0f;                          //����������=��դ��16*4
    motorStr.ReductionRatio = 2.7f;                         //������ٱ�
    motorStr.EncoderValue = 0;
    motorStr.DiameterWheel = 0.064f;//68cm                  //����ֱ��:m
    motorStr.CloseLoop = true;                              //Ĭ�ϱջ�ģʽ

}

/**
* @brief        ������PWM����
* @param        pwm��-2000~2000
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
* @brief        ����ջ��ٿ�
* @param        speed���ٶ�m/s
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
* @brief        ��������߳�
* @param
* @ref
* @author       Leo
* @note
**/
void MOTOR_Timer(void)
{
    motorStr.Counter++;
    if(motorStr.Counter >= 10)                              //�ٿ�:10ms
    {
        ENCODER_RevSample();                                //����������
        if(icarStr.sprintEnable || usbStr.connected)        //ͨ�����ӻ������Բſ����ջ�������+ʡ�磩
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
            else//�����ٷֱȿ���
            {
                if(icarStr.SpeedSet > 100)
                    icarStr.SpeedSet = 100;
                else if(icarStr.SpeedSet < -100)
                    icarStr.SpeedSet = -100;
                signed int speedRate = MOTOR_PWM_MAX/100.f*icarStr.SpeedSet; //�������ٷֱ�%

                MOTOR_SetPwmValue(speedRate);       //�����ٿ�
            }
        }
        else
        {
            MOTOR_SetPwmValue(0);
        }

        motorStr.Counter = 0;
    }
}
















