/*
 * UART.c
 *
 *  Created on: 2024��3��18��
 *      Author: Դ
 */
#include "zf_common_headfile.h"

UsbStruct usbStr;
fifo_struct uart_data_fifo;
uint8_t uart_get_data[64];
uint8_t fifo_get_data[64];
uint32_t fifo_data_count = 0;
uint8_t get_data = 0;


void USB_Edgeboard_Init(void)
{
    fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);
    //uart_init(UART_3,115200,UART3_TX_P00_0,UART3_RX_P00_1);
    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);
    //uart_rx_interrupt(UART_3, 1);
    uart_rx_interrupt(UART_2, 1);

    usbStr.counter = 0;
    usbStr.receiveFinished = false;
    usbStr.receiveStart = false;
    usbStr.receiveIndex = 0;
    usbStr.connected = false;
    usbStr.inspectorEnable = false;
}

/**
* @brief        USB-Edgeboard����һ���ֽ�����
* @param
* @ref
*
* @author       Leo
* @note
**/

void USB_Edgeboard_TransmitByte(uint8_t data)
{
    //uart_write_byte (UART_3, data);
    uart_write_byte (UART_2, data);
}

/**
* @brief        USB/UART�����жϺ���
* @param
* @ref
* @author       Leo
* @note
**/
void USART1_IRQHandler(void)
{
    uint8_t Uart1Res=0;
    //uart_query_byte(UART_3, &get_data);
    uart_query_byte(UART_2, &get_data);
    fifo_write_buffer(&uart_data_fifo, &get_data, 8);
    fifo_data_count = fifo_used(&uart_data_fifo);
    fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);
   if(fifo_data_count != 0 )
   {
       Uart1Res = *fifo_get_data;
    if(Uart1Res == USB_FRAME_HEAD && !usbStr.receiveStart)//���֡ͷ
    {
        usbStr.receiveStart = true;
        usbStr.receiveBuff[0] = Uart1Res;
        usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
        usbStr.receiveIndex = 1;
    }
    else if(usbStr.receiveIndex == 2)   //����֡����
    {
        usbStr.receiveBuff[usbStr.receiveIndex] = Uart1Res;
        usbStr.receiveIndex++;

        if(Uart1Res > USB_FRAME_LENMAX || Uart1Res < USB_FRAME_LENMIN) //֡������
        {
            usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
            usbStr.receiveIndex = 0;
            usbStr.receiveStart = false;
        }
    }
    else if(usbStr.receiveStart && usbStr.receiveIndex < USB_FRAME_LENMAX)
    {
        usbStr.receiveBuff[usbStr.receiveIndex] = Uart1Res;
        usbStr.receiveIndex++;
    }

    //����֡���
    if((usbStr.receiveIndex >= USB_FRAME_LENMAX || usbStr.receiveIndex >= usbStr.receiveBuff[2]) && usbStr.receiveIndex > USB_FRAME_LENMIN)
    {
        uint8_t check = 0;
        uint8_t length = USB_FRAME_LENMIN;

        length = usbStr.receiveBuff[2];
        for(int i=0;i<length-1;i++)
            check += usbStr.receiveBuff[i];

        if(check == usbStr.receiveBuff[length-1])//У��λ
        {
            memcpy(usbStr.receiveBuffFinished,usbStr.receiveBuff,USB_FRAME_LENMAX);
            usbStr.receiveFinished = true;

            //���ܳ�����ָ�����⴦������ʵʱ�ԣ�
            if(USB_ADDR_CONTROL  == usbStr.receiveBuffFinished[1])
            {
                Bint16_Union bint16_Union;
                Bint32_Union bint32_Union;
                for(int i=0;i<4;i++)
                bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[3+i];

                bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[7];
                bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[8];
                SERVO_SetPwmValueCorrect(bint16_Union.U16);
                icarStr.ServoPwmSet = bint16_Union.U16;         //����
                icarStr.SpeedSet = bint32_Union.Float;          //�ٶ�
            }

            if(!usbStr.connected)//��λ����������ͨ��
            {
                //RGB_SetAllColor(RGB_COLOR_GREEN);







                GPIO_BuzzerEnable(BuzzerOk);
                usbStr.connected = true;
            }

            usbStr.counterDrop = 0;
        }

        usbStr.receiveIndex = 0;
        usbStr.receiveStart = false;
    }
   }

}

/**
* @brief        �������߳̿�����
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Timr(void)
{
    if(usbStr.connected)//USBͨ�ŵ��߼��
    {
        usbStr.counterDrop++;
        if(usbStr.counterDrop >3000)//3s
        {
            usbStr.connected = false;
            usbStr.inspectorEnable = false;
            icarStr.selfcheckEnable = false;
        }

        if(usbStr.inspectorEnable)
        {
            usbStr.counterSend++;
        }
    }
}


/**
* @brief        USBͨ�Ŵ�����
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Handle(void)
{
    if(usbStr.receiveFinished)                                                              //���ճɹ�
    {
        usbStr.receiveFinished = false;
        Bint32_Union bint32_Union;
        Bint16_Union bint16_Union;

        if(usbStr.receiveBuffFinished[1] & 0x80)    //������
        {
            uint8_t Addr = (uint8_t)(usbStr.receiveBuffFinished[1] & 0x7F);
            switch(Addr)
            {
                case USB_ADDR_BATTERY :             //�����Ϣ
                    break;

                case USB_ADDR_SERVOTHRESHOLD :      //�����ֵ
                    break;
            }
        }
        else //д����
        {
            switch(usbStr.receiveBuffFinished[1])
            {
                case USB_ADDR_SERVOTHRESHOLD :   //�����ֵ
                    if(usbStr.receiveBuffFinished[3] == 1)          //��ת��ֵ
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdLeft = bint16_Union.U16;
                        flashSaveEnable = true; //�ȴ�Flash�洢
                        SERVO_SetPwmValue(servoStr.thresholdLeft);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    else if(usbStr.receiveBuffFinished[3] == 2)     //��ת��ֵ
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdRight = bint16_Union.U16;
                        flashSaveEnable = true; //�ȴ�Flash�洢
                        SERVO_SetPwmValue(servoStr.thresholdRight);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    else if(usbStr.receiveBuffFinished[3] == 3)     //��ֵ
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdMiddle = bint16_Union.U16;
                        flashSaveEnable = true; //�ȴ�Flash�洢
                        SERVO_SetPwmValue(servoStr.thresholdMiddle);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    break;

                case USB_ADDR_BUZZER :      //��������Ч
                    if(usbStr.receiveBuffFinished[3] == 1)          //OK
                        GPIO_BuzzerEnable(BuzzerOk);
                    else if(usbStr.receiveBuffFinished[3] == 2)     //Warnning
                        GPIO_BuzzerEnable(BuzzerWarnning);
                    else if(usbStr.receiveBuffFinished[3] == 3)     //Finish
                        GPIO_BuzzerEnable(BuzzerFinish);
                    else if(usbStr.receiveBuffFinished[3] == 4)     //Ding
                        GPIO_BuzzerEnable(BuzzerDing);
                    else if(usbStr.receiveBuffFinished[3] == 5)     //SystemStart
                        GPIO_BuzzerEnable(BuzzerSysStart);

                    break;

                case USB_ADDR_LIGHT :         //LED��Ч
                    for(int i=0;i<4;i++)
                        bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[i+3];

                    //RGB_SetAllColor((unsigned long)bint32_Union.U32);
                    //rgbStr.lastColor = (unsigned long)bint32_Union.U32;

                    break;

                case USB_ADDR_SPEEDMODE:        //�ٿ�ģʽ�л�
                    if(usbStr.receiveBuffFinished[3] == 1)    //����ģʽ
                        motorStr.CloseLoop = false;
                    else
                        motorStr.CloseLoop = true;

                    icarStr.SpeedSet = 0;
                    GPIO_BuzzerEnable(BuzzerDing);
                    break;


                //-----------------------------[�Լ�������]-------------------------------------------
                case USB_ADDR_INSPECTOR :           //�Լ��������
                    usbStr.inspectorEnable = true;
                    break;

                case USB_ADDR_SELFCHECK :           //��ʼ�Լ�
                    ICAR_SelfcheckControl(usbStr.receiveBuffFinished[3]);
                    break;
            }

        }
    }
    //-----------------------[�Լ�������ݷ���]-----------------------------
    if(usbStr.inspectorEnable && usbStr.connected && usbStr.counterSend > 150)//150ms
    {
        USB_Edgeboard_ServoThreshold(1);        //���Ͷ����ֵ
        system_delay_ms(1);
        USB_Edgeboard_ServoThreshold(2);
        system_delay_ms(1);
        USB_Edgeboard_ServoThreshold(3);
        system_delay_ms(1);
        USB_Edgeboard_BatteryInfo();            //���͵����Ϣ
        system_delay_ms(1);
        USB_Edgeboard_CarSpeed();               //���ͳ���
        usbStr.counterSend = 0;
    }
}


/**
* @brief        USB���Ͱ����ź�
* @param        time: ����ʱ��
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_TransmitKey(uint16_t time)
{
    uint8_t check = 0;
    uint8_t buff[8];
    Bint16_Union bint16_Union;

    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_KEYINPUT; //��ַ
    buff[2] = 0x06; //֡��

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

    for(int i=0;i<8;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

//----------------------------------------------[UNIT-���������Լ����ͨ������]----------------------------------------------------------
/**
* @brief        ���Ͷ����ֵ
* @param        chanel: 1/��ת��ֵ��2/��ת��ֵ��3/��ֵ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_ServoThreshold(uint8_t chanel)
{
    if(chanel<1 || chanel>3)
        return;

    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[9];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SERVOTHRESHOLD; //��ַ
    buff[2] = 0x07; //֡��
    buff[3] = chanel; //ͨ��

    switch(chanel)
    {
        case 1:
        {
            bint16_Union.U16 = servoStr.thresholdLeft;
            buff[4] = bint16_Union.U8_Buff[0];
            buff[5] = bint16_Union.U8_Buff[1];
            break;
        }
        case 2:
        {
            bint16_Union.U16 = servoStr.thresholdRight;
            buff[4] = bint16_Union.U8_Buff[0];
            buff[5] = bint16_Union.U8_Buff[1];
            break;
        }
        case 3:
        {
            bint16_Union.U16 = servoStr.thresholdMiddle;
            buff[4] = bint16_Union.U8_Buff[0];
            buff[5] = bint16_Union.U8_Buff[1];
            break;
        }
    }

    for(int i=0;i<6;i++)
        check += buff[i];

    buff[6] = check;

    for(int i=0;i<9;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

/**
* @brief        ���Ͱ�����Ӧ��Ϣ
* @param        time: ����ʱ��/ms
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_KeyPress(uint16_t time)
{
    if(time<100)
        return;

    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[8];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_KEYINPUT; //��ַ
    buff[2] = 0x06; //֡��

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

    for(int i=0;i<8;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}


/**
* @brief        ���͵����Ϣ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_BatteryInfo(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[11];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_BATTERY; //��ַ
    buff[2] = 0x09; //֡��

    buff[3] = icarStr.Electricity; //����
    bint32_Union.Float = icarStr.Voltage;
    buff[4] = bint32_Union.U8_Buff[0];
    buff[5] = bint32_Union.U8_Buff[1];
    buff[6] = bint32_Union.U8_Buff[2];
    buff[7] = bint32_Union.U8_Buff[3];

    for(int i=0;i<8;i++)
        check += buff[i];

    buff[8] = check;

    for(int i=0;i<11;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

/**
* @brief        ���ͳ�����Ϣ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_CarSpeed(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[10];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SPEEDBACK; //��ַ
    buff[2] = 0x08; //֡��

    bint32_Union.Float = icarStr.SpeedFeedback;
    buff[3] = bint32_Union.U8_Buff[0];
    buff[4] = bint32_Union.U8_Buff[1];
    buff[5] = bint32_Union.U8_Buff[2];
    buff[6] = bint32_Union.U8_Buff[3];

    for(int i=0;i<7;i++)
        check += buff[i];

    buff[7] = check;

    for(int i=0;i<10;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

/**
* @brief        �����Լ���Ϣ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Selfcheck(uint8_t step)
{
    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[9];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SELFCHECK; //��ַ
    buff[2] = 0x07; //֡��

    buff[3] = step;

    bint16_Union.U16 = icarStr.errorCode;
    buff[4] = bint16_Union.U8_Buff[0];
    buff[5] = bint16_Union.U8_Buff[1];

    for(int i=0;i<6;i++)
        check += buff[i];

    buff[6] = check;

    for(int i=0;i<9;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

//------------------------------------------------[END]-------------------------------------------------------------








