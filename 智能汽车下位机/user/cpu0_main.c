
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
uint8_t a=0;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���

    // �˴���д�û����� ���������ʼ�������
    FLASH_LoadAllConfig();              //����ϵͳ����
    tft180_init();
    system_delay_ms(100);                      //������ʱ���ȴ�ϵͳ�ȶ�

    ICAR_Init();                        //���ܳ�������ʼ��
    ENCODER_Init();                     //��������ʼ��
    GPIO_Initialize();                  //GPIO��ʼ��
    PID_Init();                         //PID������ʼ��
    MOTOR_Init();                       //�����ʼ��
    SERVO_Init();                       //�����ʼ��
    USB_Edgeboard_Init();               //USBͨ�ų�ʼ��
    system_delay_ms(100);               //������ʱ���ȴ�ϵͳ�ȶ�
    TIM2_Init();                        //ϵͳ���̶߳�ʱ����ʼ��
    GPIO_BuzzerEnable(BuzzerSysStart);  //������Ч

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        key_scanner();
        GPIO_Handle();                  //GPIO���ƣ�LED/������
        ICAR_Handle();                  //���ܳ�����
        FLASH_Handle();                 //Flash�洢
        USB_Edgeboard_Handle();         //USBͨ�ſ���
        tft180_show_int(0,0,icarStr.ServoPwmSet,4);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
