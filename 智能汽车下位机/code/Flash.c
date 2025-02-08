/*
 * Flash.c
 *
 *  Created on: 2024��3��20��
 *      Author: Դ
 */

#include "zf_common_headfile.h"


bool flashSaveEnable = false;

//��ʱ��ȡ����
uint8_t ArrayParams_ForFlash[FLASH_SIZE] =
{
    /*1*/       0x00,0x00,0x00,0x00,    //[Flag] [�����ֵ]
    /*2*/       0x00,0x00,0x00,0x00,    //[Flag] [�����ת��ֵ]
    /*3*/       0x00,0x00,0x00,0x00     //[Flag] [�����ת��ֵ]
};



/**
* @brief        �洢ϵͳ���ã�ȫ��/ALL��
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
                bint16_Union.U16 = servoStr.thresholdMiddle;//�����ֵPWM
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(ArrayParams_ForFlash+(i++)*4,buff,4);

    /*2*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdLeft;//�����ת��ֵ
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(ArrayParams_ForFlash+(i++)*4,buff,4);

    /*3*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdRight;//�����ת��ֵ
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(ArrayParams_ForFlash+(i++)*4,buff,4);

    FLASH_WriteBuffToFlash(0,ArrayParams_ForFlash,sizeof(ArrayParams_ForFlash));
}


/**
* @brief        ����ϵͳ���ã�ȫ��/ALL��
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

    FLASH_ReadFlashNBtye(0,ArrayParams_ForFlash,sizeof(ArrayParams_ForFlash));//��ȡFlash����

    /*1*/       memcpy(buff,ArrayParams_ForFlash+(i++)*4,4);
                if(buff[0] == FLASH_DATA_OK)                //�����ֵPWM
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
                if(buff[0] == FLASH_DATA_OK)                //�����ת��ֵ
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
                if(buff[0] == FLASH_DATA_OK)                //�����ת��ֵ
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
* @brief        ��Ƭ��flashд����
* @param        WriteAddress ƫ�Ƶ�ַ��pbuff д�������׵�ַ��num pbuff�Ĵ�С
* @ref
* @author       Leo
* @note
**/
void FLASH_WriteBuffToFlash(int WriteAddress,uint8_t * pbuff,int num)
{
    flash_write_page(0,0,pbuff,num);
}


/**
* @brief        ��flash�ж�����
* @param        ReadAddress ƫ�Ƶ�ַ��pbuff �洢�������飬ReadNum pbuff��С
* @ref
* @author       Leo
* @note
**/
void FLASH_ReadFlashNBtye(int ReadAddress, uint8_t *pbuff, int ReadNum)
{
    flash_read_page(0,0,pbuff,ReadNum);
}


/**
* @brief        Flash�����ݿ���
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


