/*
 * Icar.h
 *
 *  Created on: 2024年3月18日
 *      Author: 源
 */

#ifndef CODE_ICAR_H_
#define CODE_ICAR_H_

#include "zf_common_headfile.h"

/**
* @brief    智能车自检步骤
**/

typedef enum
{
    Selfcheck_None = 0,             //开始测试
    Selfcheck_MotorA,               //电机正转启动
    Selfcheck_MotorB,               //电机正转采样
    Selfcheck_MotorC,               //电机反转启动
    Selfcheck_MotorD,               //电机反转采样
    Selfcheck_MotorE,               //电机闭环正传启动
    Selfcheck_MotorF,               //电机闭环正传采样
    Selfcheck_MotorG,               //电机闭环反转启动
    Selfcheck_MotorH,               //电机闭环反转采样
    Selfcheck_ServoA,               //舵机测试A
    Selfcheck_Com,                  //通信测试
    Selfcheck_Buzzer,               //蜂鸣器测试
    Selfcheck_RgbLed,               //灯效测试
    Selfcheck_Key,                  //按键测试
    Selfcheck_Finish                //测试完成
}SelfcheckEnum;

/**
* @brief    智能车相关
**/
typedef struct                              //[智能车驱动主板]
{
    float Voltage;                          //电池电压
    uint8_t Electricity;                    //电池电量百分比：0~100
    float SpeedSet;                         //电机目标速度：m/s
    float SpeedFeedback;                    //电机模型实测速度：m/s
    float SpeedMaxRecords;                  //测试记录最高速
    uint16_t ServoPwmSet;                   //舵机PWM设置

    uint16_t counterKeyA;                   //按键模式A计数器
    uint16_t count;
    bool keyPressed;                        //按键按下
    bool sprintEnable;                      //闭环冲刺使能
    uint16_t counterSprint;                 //闭环冲刺时间
    uint16_t errorCode;                     //错误代码

    bool selfcheckEnable;                   //智能车自检使能
    uint16_t counterSelfcheck;              //自检计数器
    uint8_t timesSendStep;                  //发送超时数据次数
    uint16_t counterModuleCheck;            //自检计数器
    SelfcheckEnum selfcheckStep;            //自检步骤
    uint8_t speedSampleStep;                //速度采样步骤
}IcarStruct;

extern IcarStruct icarStr;


void ICAR_Init(void);
void ICAR_Timer(void);
void ICAR_Handle(void);
void ICAR_Reboot(void);

//自检相关
void ICAR_Selfcheck(void);
void ICAR_SelfcheckControl(uint8_t step);
float ICAR_SpeedSample(void);
bool ICAR_ServoCheck(void);
bool ICAR_BuzzerCheck(void);
void ICAR_RgbCheck(void);
void ICAR_SortFloat(float *buff, uint8_t start, uint8_t len);
float CMATH_AbsFloat(float data);


#endif /* CODE_ICAR_H_ */
