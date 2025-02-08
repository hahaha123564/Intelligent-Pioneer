/*
 * Encoder.c
 *
 *  Created on: 2024��3��18��
 *      Author: Դ
 */

#include "zf_common_headfile.h"

#define ProcessNiose_Encoder_Q   0.000001
#define MeasureNoise_Encoder_R  0.00005

//-------------------------------------------------------------------------------------------------------------------
// �� �� ��       : һ�׿������˲�
// ��������       : �˲�
// ��    ��       : ResrcData����Ҫ�˲���ֵ  ProcessNiose_Q������   MeasureNoise_R������
// ��    ��       :
// ��    ��       :   2021��12��12��
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
        p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
        kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
        x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ

        p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance

        p_last = p_now; //����covarianceֵ
        x_last = x_now; //����ϵͳ״ֵ̬

        return x_now;
}



/**
* @brief        ��������ʼ��
* @param
* @ref
* @author       Leo
* @note
**/

void ENCODER_Init(void)
{
    //encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);   // ��ʼ��������ģ�������� �������������ģʽ
    //encoder_clear_count(TIM2_ENCODER);                                                 // ���CNT
    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);   // ��ʼ��������ģ�������� �������������ģʽ
    encoder_clear_count(TIM5_ENCODER);                                                 // ���CNT

}


/**
* @brief        ������ת�ٲ���
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

    //PID��������������
    pidStr.vi_FeedBack = motorStr.EncoderValue;

    //����ʵ���ٶ�    ---     m/s
    icarStr.SpeedFeedback = (float)(motorStr.EncoderValue * PI * motorStr.DiameterWheel)/ MOTOR_CONTROL_CYCLE / motorStr.EncoderLine / 4.0f / motorStr.ReductionRatio; //  m/s


    if(icarStr.SpeedFeedback > 0 && icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = icarStr.SpeedFeedback;

    else if(icarStr.SpeedFeedback < 0 && -icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = -icarStr.SpeedFeedback;
}




