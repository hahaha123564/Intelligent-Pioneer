/*
 * UART.h
 *
 *  Created on: 2024��3��18��
 *      Author: Դ
 */

#ifndef CODE_UART_H_
#define CODE_UART_H_

#include "zf_common_headfile.h"

#define USB_FRAME_HEAD               0x42                //USBͨ������֡ͷ
#define USB_FRAME_LENMIN             4                   //USBͨ�������ֽ���̳���
#define USB_FRAME_LENMAX             30                  //USBͨ�������ֽ������

#define USB_ADDR_HEART               0x00                //����������
#define USB_ADDR_CONTROL             0x01                //���ܳ�����
#define USB_ADDR_SPEEDMODE           0x02                //�ٿ�ģʽ
#define USB_ADDR_SERVOTHRESHOLD      0x03                //�����ֵ
#define USB_ADDR_BUZZER              0x04                //��������Ч
#define USB_ADDR_LIGHT               0x05                //LED��Ч
#define USB_ADDR_KEYINPUT            0x06                //��������
#define USB_ADDR_BATTERY             0x07                //�����Ϣ

//�����Լ����ͨ��ʹ��
#define USB_ADDR_INSPECTOR           0x0A                //���ܳ��Լ������������
#define USB_ADDR_SELFCHECK           0x0B                //���ܳ��Լ쿪ʼ
#define USB_ADDR_SPEEDBACK           0x08                //������Ϣ������m/s

typedef struct
{
    bool receiveStart;                                      //���ݽ��տ�ʼ
    uint8_t receiveIndex;                                   //��������
    bool receiveFinished;                                   //���ݶ��н��ղ�У�����
    uint8_t receiveBuff[USB_FRAME_LENMAX];                  //USB���ն��У���ʱ����
    uint8_t receiveBuffFinished[USB_FRAME_LENMAX];          //USB���ն��У�У��ɹ�
    uint16_t counter;                                       //������
    uint16_t counterDrop;                                   //���߼�����

    uint16_t counterSend;                                   //�Լ����ݷ��ͼ�����
    bool connected;                                         //��λ��ͨ������״̬
    bool inspectorEnable;                                   //���������Լ��������ʹ��
}UsbStruct;


extern UsbStruct usbStr;


void USB_Edgeboard_Init(void);
void USB_Edgeboard_TransmitByte(uint8_t data);
void USB_Edgeboard_Timr(void);
void USB_Edgeboard_Handle(void);
void USB_Edgeboard_TransmitKey(uint16_t time);
void USART1_IRQHandler(void);

//���ܳ��Լ�
void USB_Edgeboard_ServoThreshold(uint8_t chanel);
void USB_Edgeboard_KeyPress(uint16_t time);
void USB_Edgeboard_BatteryInfo(void);
void USB_Edgeboard_CarSpeed(void);
void USB_Edgeboard_Selfcheck(uint8_t step);


#endif /* CODE_UART_H_ */
//===========================================  End Of File  ===========================================//

