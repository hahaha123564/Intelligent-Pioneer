/*
 * Flash.h
 *
 *  Created on: 2024��3��20��
 *      Author: Դ
 */

#ifndef CODE_FLASH_H_
#define CODE_FLASH_H_

#include "zf_common_headfile.h"
#define FLASH_SIZE              12
#define FLASH_ADDR_START        0x08000000+1024*63                              //���һ���������ڴ洢�û�����
#define FLASH_DATA_OK           0x5F                                            //Flash�洢��־

extern bool flashSaveEnable;

void FLASH_WriteBuffToFlash(int WriteAddress,uint8_t * pbuff,int num);
void FLASH_ReadFlashNBtye(int ReadAddress, uint8_t *pbuff, int ReadNum);
void FLASH_SaveAllConfig(void);
void FLASH_LoadAllConfig(void);
void FLASH_Handle(void);



#endif /* CODE_FLASH_H_ */
