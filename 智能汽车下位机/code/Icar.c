/*
 * Icar.c
 *
 *  Created on: 2024年3月18日
 *      Author: 源
 */

#include "zf_common_headfile.h"

IcarStruct icarStr;

/**
* @Description  : 智能车参数初始化
* @params       :
* @Date         :
* @author       : Leo
* @notes        :
**/
void ICAR_Init(void)
{
    icarStr.Electricity = 0;                    //电量信息
    icarStr.Voltage = 0;                        //电压
    icarStr.SpeedSet = 0.0f;                    //电机目标速度：m/s
    icarStr.SpeedFeedback = 0.0f;               //电机模型实测速度：m/s
    icarStr.SpeedMaxRecords = 0.0f;
    icarStr.ServoPwmSet = servoStr.thresholdMiddle;
}

/**
* @brief        智能车综合处理线程计数器
* @param
* @ref
* @author       Leo
* @note
**/
void ICAR_Timer(void)
{
    //按键
    if((1440<icarStr.ServoPwmSet)&(1500>icarStr.ServoPwmSet))
    {
        icarStr.count++;
    }
    else if((1500<icarStr.ServoPwmSet)&(1560>icarStr.ServoPwmSet))
    {
        icarStr.count++;
    }
    else
    {
        icarStr.count = 0;
    }
    if(icarStr.count>=1500)
    {
        icarStr.count = 1500;
    }

    //闭环冲刺
    if(icarStr.sprintEnable)
    {
        icarStr.counterSprint++;
        if(icarStr.counterSprint == 2000)//等待启动时间2s
        {
            //RGB_SetAllColor(RGB_COLOR_GREEN);
            icarStr.SpeedSet = 2.0f;    //电机目标速度：m/s
        }
    }
    else
    {
        icarStr.counterSprint = 0;
    }


    //自检计时
    if(icarStr.selfcheckEnable)
    {
        icarStr.counterSelfcheck++;
        icarStr.counterModuleCheck++;
        switch((uint8_t)icarStr.selfcheckStep)
        {
            case Selfcheck_None:            //开始测试
                if(icarStr.counterSelfcheck > 500)//500ms
                {
                    icarStr.selfcheckStep = Selfcheck_MotorA;
                    icarStr.counterSelfcheck = 0;
                }
                break;
            case Selfcheck_MotorA:          //电机测试A
                if(icarStr.counterSelfcheck > 2000)//2s
                {
                    icarStr.selfcheckStep = Selfcheck_MotorB;
                    icarStr.timesSendStep = 0;
                    icarStr.counterSelfcheck = 0;
                    icarStr.counterModuleCheck = 0;
                }
                break;
            case Selfcheck_MotorB:          //电机测试
                if(icarStr.counterSelfcheck > 3000)//3s
                {
                    icarStr.counterSelfcheck = 3000;
                }
                break;
            case Selfcheck_MotorC:          //电机测试
                if(icarStr.counterSelfcheck > 2000)//2s
                {
                    icarStr.selfcheckStep = Selfcheck_MotorD;
                    icarStr.timesSendStep = 0;
                    icarStr.counterSelfcheck = 0;
                    icarStr.counterModuleCheck = 0;
                }
                break;
            case Selfcheck_MotorD:          //电机测试
                if(icarStr.counterSelfcheck > 3000)//3s
                {
                    icarStr.counterSelfcheck = 3000;
                }
                break;
            case Selfcheck_MotorE:          //电机测试
                if(icarStr.counterSelfcheck > 2000)//2s
                {
                    icarStr.selfcheckStep = Selfcheck_MotorF;
                    icarStr.counterSelfcheck = 0;
                    icarStr.counterModuleCheck = 0;
                }
                break;
            case Selfcheck_MotorF:          //电机测试
                if(icarStr.counterSelfcheck > 2000)//2s
                {
                    icarStr.counterSelfcheck = 2000;
                }
                break;
            case Selfcheck_MotorG:          //电机测试
                if(icarStr.counterSelfcheck > 2000)//2s
                {
                    icarStr.selfcheckStep = Selfcheck_MotorH;
                    icarStr.counterSelfcheck = 0;
                    icarStr.counterModuleCheck = 0;
                }
                break;
            case Selfcheck_MotorH:          //电机测试
                if(icarStr.counterSelfcheck > 2000)//2s
                {
                    icarStr.counterSelfcheck = 2000;
                }
                break;
            case Selfcheck_ServoA:          //舵机测试
                if(icarStr.counterSelfcheck > 5000)//2s
                {
                    icarStr.counterSelfcheck = 5000;
                }
                break;
            case Selfcheck_Com:             //通信测试
                if(icarStr.counterSelfcheck > 1000)//1s
                {
                    icarStr.selfcheckStep = Selfcheck_Buzzer;
                    icarStr.counterSelfcheck = 0;
                    icarStr.counterModuleCheck = 0;
                }
                break;
            case Selfcheck_Buzzer:          //蜂鸣器测试
                if(icarStr.counterSelfcheck > 5000)//2s
                {
                    icarStr.counterSelfcheck = 5000;
                }
                break;
            case Selfcheck_RgbLed:          //灯效测试
                if(icarStr.counterSelfcheck > 4000)//2s
                {
                    icarStr.counterSelfcheck = 4000;
                }
                break;
            case Selfcheck_Key:             //按键测试
                if(icarStr.counterSelfcheck > 5000)//2s
                {
                    icarStr.counterSelfcheck = 5000;
                }
                break;

        }
    }
}

/**
* @Description  : 智能车综合处理函数
* @params       :
* @Date         :
* @author       : Leo
* @notes        :
**/
void ICAR_Handle(void)
{
    //按键挡位操作
    /*if(icarStr.keyPressed)
    {
        if(icarStr.counterKeyA > 100)   //100ms有效
            USB_Edgeboard_TransmitKey(icarStr.counterKeyA); //发送按键时长

        //闭环冲刺
        if(icarStr.counterKeyA > 2000 && !icarStr.selfcheckEnable)//按下2s，闭环冲刺
        {
            icarStr.counterSprint = 0;
            icarStr.sprintEnable = true;
            GPIO_BuzzerEnable(BuzzerOk);
        }

        icarStr.counterKeyA = 0;
        icarStr.keyPressed = false;

        if(icarStr.selfcheckEnable)//自检测试按键好坏
        {
            icarStr.errorCode &= ~(1<<8);
        }
    }*/

    //闭环冲刺（测试电机）
    if(icarStr.sprintEnable)
    {
        if(icarStr.counterSprint > 3000)//冲刺时间：3s
        {
            GPIO_BuzzerEnable(BuzzerFinish);
            //RGB_SetAllColor(RGB_COLOR_RED);
            icarStr.SpeedSet = 0.f;    //电机目标速度：m/s
            icarStr.sprintEnable = false;
        }
    }

    //智能车自检
    if(icarStr.selfcheckEnable)
    {
        ICAR_Selfcheck();
    }
}

/**
* @brief        控制板重启
* @param
* @ref
* @author       Leo
* @note
**/
void ICAR_Reboot(void)
{
    /*__disable_fault_irq();
    NVIC_SystemReset();*/
}


//----------------------------------------------[UNIT-智能汽车自检相关]----------------------------------------------------------
/**
* @brief        智能车自检
* @param
* @ref
* @author       Leo
* @note
**/
void ICAR_Selfcheck(void)
{
    static bool errorMotor;
    static bool errorPID;
    switch(icarStr.selfcheckStep)
    {
        case Selfcheck_None:            //开始测试
            motorStr.CloseLoop = false;
            icarStr.timesSendStep = 0;
            break;
        case Selfcheck_MotorA:          //电机正转启动
            motorStr.CloseLoop = false;
            icarStr.SpeedSet = 25.0f;   //25%
            icarStr.errorCode |= (1<<1);
            icarStr.errorCode |= (1<<2);
            errorMotor = true;
            break;
        case Selfcheck_MotorB:          //电机正转采样
            if(ICAR_SpeedSample()>0.5)
            {
                icarStr.errorCode &= ~(1<<1);
                icarStr.errorCode &= ~(1<<2);
                icarStr.selfcheckStep = Selfcheck_MotorC;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorB);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                errorMotor = false;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 3000)//超时
            {
                if(icarStr.timesSendStep < 5)
                {
                    USB_Edgeboard_Selfcheck(Selfcheck_MotorB);
                    icarStr.timesSendStep++;
                }
                icarStr.counterSelfcheck = 0;
            }
            break;
        case Selfcheck_MotorC:          //电机反转启动
            motorStr.CloseLoop = false;
            icarStr.SpeedSet = -25.0f;
            break;
        case Selfcheck_MotorD:          //电机反转采样
            if(ICAR_SpeedSample()<-0.5)
            {
                if(!errorMotor)
                    icarStr.errorCode &= ~(1<<1);

                icarStr.selfcheckStep = Selfcheck_MotorE;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorD);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 3000)//超时
            {
                icarStr.errorCode |= (1<<1);

                if(icarStr.timesSendStep < 5)
                {
                    USB_Edgeboard_Selfcheck(Selfcheck_MotorD);
                    icarStr.timesSendStep++;
                }
            }
            break;
        case Selfcheck_MotorE:          //电机闭环正传启动
            icarStr.SpeedSet = 1.0f;
            motorStr.CloseLoop = true;
            icarStr.errorCode |= (1<<3);
            errorPID = true;
            break;
        case Selfcheck_MotorF:          //电机闭环正传采样
            if( CMATH_AbsFloat(ICAR_SpeedSample()-icarStr.SpeedSet) < 0.3)
            {
                icarStr.errorCode &= ~(1<<3);
                icarStr.selfcheckStep = Selfcheck_MotorG;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorF);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                errorPID = false;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 2000)//超时
            {
                USB_Edgeboard_Selfcheck(Selfcheck_MotorF);
                icarStr.selfcheckStep = Selfcheck_ServoA;
                icarStr.timesSendStep = 0;
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
            }
            break;
        case Selfcheck_MotorG:          //电机闭环反转启动
            motorStr.CloseLoop = true;
            icarStr.SpeedSet = -1.0f;
            icarStr.errorCode |= (1<<3);
            icarStr.timesSendStep = 0;
            break;
        case Selfcheck_MotorH:          //电机闭环反转采样
            if( CMATH_AbsFloat(ICAR_SpeedSample()-icarStr.SpeedSet) < 0.3 && !errorPID)
            {
                icarStr.errorCode &= ~(1<<3);
                icarStr.selfcheckStep = Selfcheck_ServoA;
                icarStr.timesSendStep = 0;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorH);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 2000)//超时
            {
                USB_Edgeboard_Selfcheck(Selfcheck_MotorH);
                icarStr.selfcheckStep = Selfcheck_ServoA;
                icarStr.timesSendStep = 0;
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
            }
            break;

        case Selfcheck_ServoA:          //舵机测试
            motorStr.CloseLoop = false;
            icarStr.SpeedSet = 0.0f;
            icarStr.timesSendStep = 0;
            ICAR_ServoCheck();
            if(icarStr.counterSelfcheck >= 2000 && icarStr.timesSendStep < 5)//超时
            {
                USB_Edgeboard_Selfcheck(Selfcheck_ServoA);
                icarStr.timesSendStep++;
            }
            break;

        case Selfcheck_Com:             //通信测试
            icarStr.ServoPwmSet = servoStr.thresholdMiddle;
            SERVO_AngleControl(icarStr.ServoPwmSet);
            icarStr.selfcheckStep = Selfcheck_Buzzer;
            break;

        case Selfcheck_Buzzer:          //蜂鸣器测试
            ICAR_BuzzerCheck();
            if(icarStr.counterSelfcheck >= 1000 && icarStr.timesSendStep < 5)//超时
            {
                USB_Edgeboard_Selfcheck(Selfcheck_Buzzer);
                icarStr.timesSendStep++;
            }
            break;
        case Selfcheck_RgbLed:          //灯效测试
            ICAR_RgbCheck();
            if(icarStr.counterSelfcheck >= 1000 && icarStr.timesSendStep < 5)//超时
            {
                USB_Edgeboard_Selfcheck(Selfcheck_RgbLed);
                icarStr.timesSendStep++;
            }
            break;
        case Selfcheck_Key:             //按键测试
            if(icarStr.counterSelfcheck >= 1000 && icarStr.timesSendStep < 5)//超时
            {
                USB_Edgeboard_Selfcheck(Selfcheck_Key);
                icarStr.timesSendStep++;
            }
            break;
        case Selfcheck_Finish:          //测试完成
            GPIO_BuzzerEnable(BuzzerFinish);
            icarStr.selfcheckEnable = false;
            ICAR_Reboot();//重启
            break;
    }

}


/**
* @brief        自检流程控制
* @param
* @ref
* @author       Leo
* @note
**/
uint8_t resStep = 0;
void ICAR_SelfcheckControl(uint8_t step)
{
    resStep = step;
    if(step == (uint8_t)Selfcheck_None)
    {
        //RGB_SetAllColor(RGB_COLOR_GREEN);   //开启绿色灯光
        GPIO_BuzzerEnable(BuzzerSysStart);
        icarStr.selfcheckEnable = true;
        icarStr.counterSelfcheck = 0;
        icarStr.selfcheckStep = Selfcheck_None;
        icarStr.errorCode |= (1<<8);
    }
    else
    {
        icarStr.counterSelfcheck = 0;
        icarStr.counterModuleCheck = 0;
        icarStr.selfcheckStep = resStep;
        icarStr.timesSendStep = 0;
        GPIO_BuzzerEnable(BuzzerOk);
    }
}

/**
* @brief        闭环速度采样
* @param
* @ref
* @author       Leo
* @note
**/
float buffSample[5] = {0};
float ICAR_SpeedSample(void)
{
    if(icarStr.counterModuleCheck>100)//100ms
    {
        switch(icarStr.speedSampleStep)
        {
            case 0:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 1:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 2:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 3:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 4:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep = 0;
                ICAR_SortFloat(buffSample,0,5);
                return buffSample[2];
                break;
        }
        icarStr.counterModuleCheck = 0;
    }

    return 0;
}

/**
* @brief        舵机测试
* @param
* @ref
* @author       Leo
* @note
**/
bool ICAR_ServoCheck(void)
{
    if(icarStr.counterModuleCheck < 1)
        icarStr.ServoPwmSet = servoStr.thresholdLeft;

    if(icarStr.counterModuleCheck>100)//100ms
    {
        icarStr.ServoPwmSet +=30;
        if(icarStr.ServoPwmSet >= servoStr.thresholdRight)
        {
            icarStr.ServoPwmSet = servoStr.thresholdLeft;
            return true;
        }
        SERVO_SetPwmValueCorrect(icarStr.ServoPwmSet);
        icarStr.counterModuleCheck = 1;
    }

    return false;
}


/**
* @brief        蜂鸣器测试
* @param
* @ref
* @author       Leo
* @note
**/
uint8_t indexBuzzerCheck = 0;
bool ICAR_BuzzerCheck(void)
{
    if(icarStr.counterModuleCheck>800)
    {
        indexBuzzerCheck++;
        switch(indexBuzzerCheck)
        {
            case 1:
                GPIO_BuzzerEnable(BuzzerDing);
                break;
            case 2:
                GPIO_BuzzerEnable(BuzzerOk);
                break;
            case 3:
                GPIO_BuzzerEnable(BuzzerSysStart);
                break;
            case 4:
                GPIO_BuzzerEnable(BuzzerWarnning);
                break;
            case 5:
                GPIO_BuzzerEnable(BuzzerFinish);
                indexBuzzerCheck = 0;
                return true;
        }
        icarStr.counterModuleCheck = 0;
    }

    return false;
}

/**
* @brief        RGB灯测试
* @param
* @ref
* @author       Leo
* @note
**/
uint8_t indexRgbCheck = 0;
void ICAR_RgbCheck(void)
{
    if(icarStr.counterModuleCheck>300)
    {
        indexRgbCheck++;
        switch(indexRgbCheck)
        {
            case 1:
                //RGB_SetAllColor(RGB_COLOR_RED);
            break;
            case 2:
                //RGB_SetAllColor(RGB_COLOR_GREEN);
            break;
            case 3:
                //RGB_SetAllColor(RGB_COLOR_BLUE);
                indexRgbCheck = 0;
            break;
        }
        icarStr.counterModuleCheck = 0;
    }
}
/**
* @brief        浮点数排序
* @param        buff:数据存储数组，start：起始位，len：排序长度
* @ref
* @author       Leo
* @note
**/
void ICAR_SortFloat(float *buff, uint8_t start, uint8_t len)
{
    float n = 0;
    for(uint8_t i = start; i < start+len; i++)
    {
        for(uint8_t j = i+1; j<start+len; j++)
        {
            if(buff[i] > buff[j])
            {
                n = buff[i];
                buff[i] = buff[j];
                buff[j] = n;
            }
        }
    }
}


/**
* @brief        浮点数绝对值函数
* @param        data：要求绝对值的单精度浮点数
* @ref
* @author       Leo
* @note
**/
float CMATH_AbsFloat(float data)
{
    if(data < 0)
        return -data;
    else
        return data;
}












