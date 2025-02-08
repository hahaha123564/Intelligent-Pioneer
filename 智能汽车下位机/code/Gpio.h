/*
 * Gpio.h
 *
 *  Created on: 2024��3��18��
 *      Author: Դ
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
* @brief    ��������Ч
**/
typedef enum
{
    BuzzerOk = 0,                       //ȷ����ʾ��
    BuzzerWarnning,                     //������ʾ��
    BuzzerSysStart,                     //������ʾ��
    BuzzerDing,                         //��=====(������*)
    BuzzerFinish,                       //������ʾ��
}BuzzerEnum;


/**
* @brief    ������LED���
**/
typedef struct
{
    bool KeyPress;                      //��������-B
    uint16_t CounterLed;                //LED��˸������
}GpioStruct;


/**
* @brief    ���������
**/
typedef struct
{
    bool Enable;                        //ʹ�ܱ�־
    uint16_t Times;                     //���д���
    uint16_t Counter;                   //������
    uint16_t Cut;                       //���ʱ��
    bool Silent;                        //�Ƿ���÷�����
}BuzzerStruct;


extern GpioStruct gpioStr;
extern BuzzerStruct buzzerStr;

void GPIO_Initialize(void);
void GPIO_Timer(void);
void GPIO_Handle(void);
void GPIO_BuzzerEnable(BuzzerEnum buzzer);
void EXTI2_IRQHandler(void);









#endif /* CODE_GPIO_H_ */
