/*
 * Gpio.h
 *
 *  Created on: 2024年3月18日
 *      Author: 源
 */

#ifndef CODE_GPIO_H_
#define CODE_GPIO_H_

#include "zf_common_headfile.h"

#define BUZZER_PIN              (P33_10)
//#define BUZZER_PIN              (P20_13)
#define LED                     (P20_9)
#define KEY                     (P20_6)

#define LED_ON          (gpio_set_level(LED, 0))
#define LED_OFF         (gpio_set_level(LED, 1))
#define LED_REV         (gpio_toggle_level(LED))

#define BUZZER_ON       (gpio_set_level(BUZZER_PIN, 1))
#define BUZZER_OFF      (gpio_set_level(BUZZER_PIN, 0))
#define BUZZER_REV      (gpio_toggle_level(BUZZER_PIN))
/**
* @brief    蜂鸣器音效
**/
typedef enum
{
    BuzzerOk = 0,                       //确认提示音
    BuzzerWarnning,                     //报警提示音
    BuzzerSysStart,                     //开机提示音
    BuzzerDing,                         //叮=====(￣▽￣*)
    BuzzerFinish,                       //结束提示音
}BuzzerEnum;


/**
* @brief    按键和LED相关
**/
typedef struct
{
    bool KeyPress;                      //按键输入-B
    uint16_t CounterLed;                //LED闪烁计数器
}GpioStruct;


/**
* @brief    蜂鸣器相关
**/
typedef struct
{
    bool Enable;                        //使能标志
    uint16_t Times;                     //鸣叫次数
    uint16_t Counter;                   //计数器
    uint16_t Cut;                       //间隔时间
    bool Silent;                        //是否禁用蜂鸣器
}BuzzerStruct;


extern GpioStruct gpioStr;
extern BuzzerStruct buzzerStr;

void GPIO_Initialize(void);
void GPIO_Timer(void);
void GPIO_Handle(void);
void GPIO_BuzzerEnable(BuzzerEnum buzzer);
void EXTI2_IRQHandler(void);









#endif /* CODE_GPIO_H_ */
