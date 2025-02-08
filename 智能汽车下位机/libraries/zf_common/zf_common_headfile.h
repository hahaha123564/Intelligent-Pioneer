
#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_



//===================================================C语言 函数库===================================================
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
//===================================================个人 函数库===================================================
#include "UART.h"
#include "Icar.h"
#include "Encoder.h"
#include "Gpio.h"
#include "Servo.h"
#include "Pid.h"
#include "Motor.h"
#include "Flash.h"
#include "Pit.h"
//===================================================芯片 SDK 底层===================================================
#include "ifxAsclin_reg.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxCcu6_Timer.h"
#include "IfxScuEru.h"
//===================================================芯片 SDK 底层===================================================

//====================================================开源库公共层====================================================
#include "zf_common_typedef.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "isr_config.h"
//====================================================开源库公共层====================================================

//===================================================芯片外设驱动层===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dma.h"
#include "zf_driver_encoder.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_spi.h"
#include "zf_driver_soft_spi.h"
#include "zf_driver_uart.h"
#include "zf_driver_timer.h"
//===================================================芯片外设驱动层===================================================

//===================================================外接设备驱动层===================================================
#include "zf_device_absolute_encoder.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_gps_tau1201.h"
#include "zf_device_camera.h"
#include "zf_device_detector.h"
#include "zf_device_dl1a.h"
#include "zf_device_dl1b.h"
#include "zf_device_icm20602.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu963ra.h"
#include "zf_device_ips114.h"
#include "zf_device_ips200.h"
#include "zf_device_key.h"
#include "zf_device_mpu6050.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_oled.h"
#include "zf_device_ov7725.h"
#include "zf_device_scc8660.h"
#include "zf_device_tft180.h"
#include "zf_device_tsl1401.h"
#include "zf_device_type.h"
#include "zf_device_virtual_oscilloscope.h"
#include "zf_device_wifi_uart.h"
#include "zf_device_wifi_spi.h"
#include "zf_device_wireless_uart.h"
//===================================================外接设备驱动层===================================================

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))


//IO口操作,只对单一的IO口!
//确保n的值小于16!


typedef union
{
    uint8_t U8_Buff[2];
    uint16_t U16;
    uint16_t S16;
}Bint16_Union;

typedef union
{
    uint8_t U8_Buff[4];
    float Float;
    unsigned long U32;
}Bint32_Union;



#endif

