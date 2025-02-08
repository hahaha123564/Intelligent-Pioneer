
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
uint8_t a=0;

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口

    // 此处编写用户代码 例如外设初始化代码等
    FLASH_LoadAllConfig();              //加载系统配置
    tft180_init();
    system_delay_ms(100);                      //启动延时：等待系统稳定

    ICAR_Init();                        //智能车参数初始化
    ENCODER_Init();                     //编码器初始化
    GPIO_Initialize();                  //GPIO初始化
    PID_Init();                         //PID参数初始化
    MOTOR_Init();                       //电机初始化
    SERVO_Init();                       //舵机初始化
    USB_Edgeboard_Init();               //USB通信初始化
    system_delay_ms(100);               //启动延时：等待系统稳定
    TIM2_Init();                        //系统主线程定时器初始化
    GPIO_BuzzerEnable(BuzzerSysStart);  //开机音效

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        key_scanner();
        GPIO_Handle();                  //GPIO控制：LED/蜂鸣器
        ICAR_Handle();                  //智能车控制
        FLASH_Handle();                 //Flash存储
        USB_Edgeboard_Handle();         //USB通信控制
        tft180_show_int(0,0,icarStr.ServoPwmSet,4);
        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
