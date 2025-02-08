/*
 * Flash.c
 *
 *  Created on: 2024年3月20日
 *      Author: 源
 */

#include "zf_common_headfile.h"


bool flashSaveEnable = false;

//临时存取数组
uint8_t ArrayParams_ForFlash[FLASH_SIZE] =
{
    /*1*/       0x00,0x00,0x00,0x00,    //[Flag] [舵机中值]
    /*2*/       0x00,0x00,0x00,0x00,    //[Flag] [舵机左转阈值]
    /*3*/       0x00,0x00,0x00,0x00     //[Flag] [舵机右转阈值]
};



/**
* @brief        存储系统配置（全部/ALL）
* @param
* @ref
* @author       Leo
* @note
**/
void FLASH_SaveAllConfig(void)
{
    int i = 0;
    uint8_t buff[4] = {0};
    Bint16_Union bint16_Union;

    memset(ArrayParams_ForFlash,0,sizeof(ArrayParams_ForFlash));

    /*1*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdMiddle;//舵机中值PWM
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(ArrayParams_ForFlash+(i++)*4,buff,4);

    /*2*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdLeft;//舵机左转阈值
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(ArrayParams_ForFlash+(i++)*4,buff,4);

    /*3*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdRight;//舵机右转阈值
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(ArrayParams_ForFlash+(i++)*4,buff,4);

    FLASH_WriteBuffToFlash(0,ArrayParams_ForFlash,sizeof(ArrayParams_ForFlash));
}


/**
* @brief        加载系统配置（全部/ALL）
* @param
* @ref
* @author       Leo
* @note
**/
void FLASH_LoadAllConfig(void)
{
    int i = 0;
    Bint16_Union bint16_Union;
    uint8_t buff[4];

    FLASH_ReadFlashNBtye(0,ArrayParams_ForFlash,sizeof(ArrayParams_ForFlash));//读取Flash数据

    /*1*/       memcpy(buff,ArrayParams_ForFlash+(i++)*4,4);
                if(buff[0] == FLASH_DATA_OK)                //舵机中值PWM
                {
                    bint16_Union.U8_Buff[0] = buff[1];
                    bint16_Union.U8_Buff[1] = buff[2];
                    servoStr.thresholdMiddle = bint16_Union.U16;
                }
                else
                {
                    servoStr.thresholdMiddle = SERVO_PWM_MIDDLE;
                }

    /*2*/       memcpy(buff,ArrayParams_ForFlash+(i++)*4,4);
                if(buff[0] == FLASH_DATA_OK)                //舵机左转阈值
                {
                    bint16_Union.U8_Buff[0] = buff[1];
                    bint16_Union.U8_Buff[1] = buff[2];
                    servoStr.thresholdLeft = bint16_Union.U16;
                }
                else
                {
                    servoStr.thresholdLeft = SERVO_PWM_MAX_L;
                }

    /*3*/       memcpy(buff,ArrayParams_ForFlash+(i++)*4,4);
                if(buff[0] == FLASH_DATA_OK)                //舵机右转阈值
                {
                    bint16_Union.U8_Buff[0] = buff[1];
                    bint16_Union.U8_Buff[1] = buff[2];
                    servoStr.thresholdRight = bint16_Union.U16;
                }
                else
                {
                    servoStr.thresholdRight = SERVO_PWM_MAX_R;
                }
}

/**
* @brief        向片内flash写数据
* @param        WriteAddress 偏移地址，pbuff 写入数据首地址，num pbuff的大小
* @ref
* @author       Leo
* @note
**/
void FLASH_WriteBuffToFlash(int WriteAddress,uint8_t * pbuff,int num)
{
    flash_write_page(0,0,pbuff,num);
}


/**
* @brief        从flash中读数据
* @param        ReadAddress 偏移地址，pbuff 存储数据数组，ReadNum pbuff大小
* @ref
* @author       Leo
* @note
**/
void FLASH_ReadFlashNBtye(int ReadAddress, uint8_t *pbuff, int ReadNum)
{
    flash_read_page(0,0,pbuff,ReadNum);
}


/**
* @brief        Flash存数据控制
* @param
* @ref
* @author       Leo
* @note
**/
void FLASH_Handle(void)
{
    if(flashSaveEnable)
    {
        FLASH_SaveAllConfig();
        flashSaveEnable = false;
    }
}


