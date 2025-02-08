/*
 * Gpio.c
 *
 *  Created on: 2024年3月18日
 *      Author: 源
 */

#include "zf_common_headfile.h"

GpioStruct gpioStr;
BuzzerStruct buzzerStr;


/**
* @brief        GPIO外设初始化
* @param
* @ref
* @author       Leo
* @note         外设：蜂鸣器 x1  状态LED x1  按键输入 x1
**/
void GPIO_Initialize(void)
{
    //gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);          // 初始化 蜂鸣器 输出 默认高电平 推挽输出模式
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);          // 初始化 蜂鸣器 输出 默认高电平 推挽输出模式
    gpio_init(LED, GPO, 0, GPO_PUSH_PULL);                 // 初始化 LED 输出 默认高电平 推挽输出模式
    gpio_init(KEY, GPI, GPIO_HIGH, GPI_PULL_UP);          // 初始化 KEY1 输入 默认高电平 上拉输入
    key_init(10);

    buzzerStr.Counter = 0;
    buzzerStr.Cut = 0;
    buzzerStr.Enable = false;
    buzzerStr.Times = 0;
}

/**
* @brief        GPIO线程控制器
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Timer(void)
{
    //蜂鸣器控制
    if(buzzerStr.Enable)
    {
        buzzerStr.Counter++;

        if(buzzerStr.Cut<buzzerStr.Counter)
            buzzerStr.Counter = buzzerStr.Cut;
    }

    //LED闪烁
    gpioStr.CounterLed++;
}



/**
* @brief        GPIO逻辑处理函数
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Handle(void)
{
    //蜂鸣器控制
    if(buzzerStr.Enable && !buzzerStr.Silent)
    {
        if(buzzerStr.Times<=0)
        {
            BUZZER_OFF;
            buzzerStr.Enable = false;
            return;
        }
        else if(buzzerStr.Cut<=buzzerStr.Counter)
        {
            BUZZER_REV;
            buzzerStr.Times--;
            buzzerStr.Counter = 0;
        }
    }
    else
        BUZZER_OFF;

    //LED控制
    if(gpioStr.CounterLed > 100)        //100ms
    {
        LED_REV;
        gpioStr.CounterLed = 0;
    }
}


/**
* @brief        蜂鸣器使能
* @param        buzzer：蜂鸣器工作模式
* @ref
* @author       Leo
* @note
**/
void GPIO_BuzzerEnable(BuzzerEnum buzzer)
{
    switch(buzzer)
    {
        case BuzzerOk:
            buzzerStr.Cut = 70;         //70ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 4;
            break;

        case BuzzerWarnning:
            buzzerStr.Cut = 100;        //100ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 10;
            break;

        case BuzzerSysStart:
            buzzerStr.Cut = 60;         //60ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 6;
            break;

        case BuzzerDing:
            buzzerStr.Cut = 30;         //30ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 2;
            break;

        case BuzzerFinish:
            buzzerStr.Cut = 200;        //200ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 6;
            break;
    }

    buzzerStr.Counter = 0;
}


/**
* @brief        按键A中断响应函数
* @param
* @ref
* @author       Leo
* @note
**/
void EXTI2_IRQHandler(void)
{
        if((key_get_state(KEY_1)==KEY_SHORT_PRESS)||(key_get_state(KEY_1)==KEY_LONG_PRESS))    //按键按下
        {
            gpioStr.KeyPress = true;
            GPIO_BuzzerEnable(BuzzerDing);
            key_clear_all_state();
        }
        else    //按键弹起
        {
            gpioStr.KeyPress = false;

        }

}











