/*
 * Gpio.c
 *
 *  Created on: 2024��3��18��
 *      Author: Դ
 */

#include "zf_common_headfile.h"

GpioStruct gpioStr;
BuzzerStruct buzzerStr;


/**
* @brief        GPIO�����ʼ��
* @param
* @ref
* @author       Leo
* @note         ���裺������ x1  ״̬LED x1  �������� x1
**/
void GPIO_Initialize(void)
{
    //gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);          // ��ʼ�� ������ ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);          // ��ʼ�� ������ ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED, GPO, 0, GPO_PUSH_PULL);                 // ��ʼ�� LED ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(KEY, GPI, GPIO_HIGH, GPI_PULL_UP);          // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
    key_init(10);

    buzzerStr.Counter = 0;
    buzzerStr.Cut = 0;
    buzzerStr.Enable = false;
    buzzerStr.Times = 0;
}

/**
* @brief        GPIO�߳̿�����
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Timer(void)
{
    //����������
    if(buzzerStr.Enable)
    {
        buzzerStr.Counter++;

        if(buzzerStr.Cut<buzzerStr.Counter)
            buzzerStr.Counter = buzzerStr.Cut;
    }

    //LED��˸
    gpioStr.CounterLed++;
}



/**
* @brief        GPIO�߼�������
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Handle(void)
{
    //����������
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

    //LED����
    if(gpioStr.CounterLed > 100)        //100ms
    {
        LED_REV;
        gpioStr.CounterLed = 0;
    }
}


/**
* @brief        ������ʹ��
* @param        buzzer������������ģʽ
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
* @brief        ����A�ж���Ӧ����
* @param
* @ref
* @author       Leo
* @note
**/
void EXTI2_IRQHandler(void)
{
        if((key_get_state(KEY_1)==KEY_SHORT_PRESS)||(key_get_state(KEY_1)==KEY_LONG_PRESS))    //��������
        {
            gpioStr.KeyPress = true;
            GPIO_BuzzerEnable(BuzzerDing);
            key_clear_all_state();
        }
        else    //��������
        {
            gpioStr.KeyPress = false;

        }

}











