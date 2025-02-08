/*
 * Encoder.c
 *
 *  Created on: 2024年3月18日
 *      Author: 源
 */

#include "zf_common_headfile.h"

#define ProcessNiose_Encoder_Q   0.000001
#define MeasureNoise_Encoder_R  0.00005

//-------------------------------------------------------------------------------------------------------------------
// 函 数 名       : 一阶卡尔曼滤波
// 函数功能       : 滤波
// 输    入       : ResrcData：需要滤波的值  ProcessNiose_Q：噪声   MeasureNoise_R：噪声
// 输    出       :
// 日    期       :   2021年12月12日
//-------------------------------------------------------------------------------------------------------------------
float KalmanFilterEncoderLeft(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
{
        float R = MeasureNoise_R;
        float Q = ProcessNiose_Q;

        static        float x_last;

        float x_mid = x_last;
        float x_now;

        static        float p_last;

        float p_mid ;
        float p_now;
        float kg;

        x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
        p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
        kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
        x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值

        p_now=(1-kg)*p_mid;//最优值对应的covariance

        p_last = p_now; //更新covariance值
        x_last = x_now; //更新系统状态值

        return x_now;
}



/**
* @brief        编码器初始化
* @param
* @ref
* @author       Leo
* @note
**/

void ENCODER_Init(void)
{
    //encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);   // 初始化编码器模块与引脚 正交解码编码器模式
    //encoder_clear_count(TIM2_ENCODER);                                                 // 清空CNT
    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);   // 初始化编码器模块与引脚 正交解码编码器模式
    encoder_clear_count(TIM5_ENCODER);                                                 // 清空CNT

}


/**
* @brief        编码器转速采样
* @param
* @ref
* @author       Leo
* @note
**/
void ENCODER_RevSample(void)
{
    float encoder_v;
    //encoder_v = encoder_get_count(TIM2_ENCODER);
    encoder_v = -encoder_get_count(TIM5_ENCODER);

    motorStr.EncoderValue=KalmanFilterEncoderLeft(encoder_v,ProcessNiose_Encoder_Q,MeasureNoise_Encoder_R);
    //encoder_clear_count(TIM2_ENCODER);
    encoder_clear_count(TIM5_ENCODER);

    if(motorStr.EncoderValue > 32767)
        motorStr.EncoderValue = motorStr.EncoderValue - 65536;

    //PID负反馈数据输入
    pidStr.vi_FeedBack = motorStr.EncoderValue;

    //计算实际速度    ---     m/s
    icarStr.SpeedFeedback = (float)(motorStr.EncoderValue * PI * motorStr.DiameterWheel)/ MOTOR_CONTROL_CYCLE / motorStr.EncoderLine / 4.0f / motorStr.ReductionRatio; //  m/s


    if(icarStr.SpeedFeedback > 0 && icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = icarStr.SpeedFeedback;

    else if(icarStr.SpeedFeedback < 0 && -icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = -icarStr.SpeedFeedback;
}




