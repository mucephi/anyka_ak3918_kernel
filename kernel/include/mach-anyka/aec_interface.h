
/**
* @file	aec_interface.h
* @brief Anyka AEC Module interfaces header file.
*
* This file declare Anyka AEC Module interfaces.
* Copyright (C) 2011 Anyka (Guangzhou) Microelectronics Technology Co., Ltd.
* @author	Tang Xuechai
* @date	2013-07-15
* @version V1.0.0
* @ref
*/

#ifndef _AEC_INTERFACE_H_
#define _AEC_INTERFACE_H_

#include "anyka_types.h"

#define  AECS_VERSION_STRING (T_U8 *)"EchoS Version V1.0.04_svn95"


typedef T_pVOID (*AEC_CALLBACK_FUN_MALLOC)(T_U32 size); 
typedef T_VOID  (*AEC_CALLBACK_FUN_FREE)(T_pVOID mem);
typedef T_VOID  (*AEC_CALLBACK_FUN_PRINTF)(T_pCSTR format, ...);
typedef T_VOID  (*AEC_CALLBACK_FUN_RTC_DELAY) (T_U32 ulTicks);
typedef T_VOID  (*AEC_CALLBACK_FUN_NOTIFY) (T_U32 event);

typedef enum
{
    AEC_TYPE_UNKNOWN,
    AEC_TYPE_1,
    AEC_TYPE_2
}T_AEC_TYPE;

typedef struct
{
    AEC_CALLBACK_FUN_MALLOC			Malloc;
    AEC_CALLBACK_FUN_FREE			Free;
    AEC_CALLBACK_FUN_PRINTF			printf;
    AEC_CALLBACK_FUN_RTC_DELAY		delay;
    AEC_CALLBACK_FUN_NOTIFY         notify;
}T_AEC_CB_FUNS;

typedef struct
{
    T_U32	m_Type;				//��չ�ã�����AEC�㷨�����T_AEC_TYPE��ö��֮һ
    T_U32	m_SampleRate;		//sample rate, sample per second
    T_U16	m_Channels;			//channel number
    T_U16	m_BitsPerSample;	//bits per sample  

    union 
    {
        struct 
        {
            T_U32   m_aecBypass;
            T_U32   m_framelen;  //NN
            T_U32   m_tail;      //TAIL 

            /* ADC���ڲ��� */
            /* adc������ֵ�� Ĭ��ֵ�� 512����mic�������ȹ�ϵ�Ƚϴ�*/
            T_U32   AdcMinSpeechPow;
            /* adc�������������Ĺ���ʱ�䣬����(ms)Ϊ��λ��Ĭ��ֵ�� 920�����������������Լ��Ƿ�ɱ���ϡ�*/
            T_U32   AdcSpeechHoldTime;  
            /* adc���ڻ�������ϵ����Ĭ���� (T_U32)(1.8*(1<<14))��
               ����Խ�����ֵҪԽС����Ȼ���ܻ���Ҫ�ʵ����AdcMinSpeechPow */
            T_U32   AdcSpeechMultiple;
            /* adc�����ı�ʱ������ʱ�䣬����(ms)Ϊ��λ��Ĭ��ֵ�� 5000 */
            T_S32   AdcConvergTime;
            /* adc��ʼһ��ʱ������ݶ������������ö�����ʱ�䳤�ȣ�����(ms)Ϊ��λ��Ĭ��ֵ�� 100 */
            T_U32   AdcCutTime; 

            /* DAC���ڲ��� */
            /*  �Է��ֻ��˴�������������ֵ�� Ĭ��ֵ�� 512 */
            T_U32   DacMinSpeechPow;
            /* �Է��ֻ��˴��������������������������Ĺ���ʱ�䣬����(ms)Ϊ��λ��Ĭ��ֵ�� 920 */
            T_U32   DacSpeechHoldTime; 
            /* �Է��ֻ������ڻ�������ϵ����Ĭ����(T_U32)(1.8*(1<<14))��
               ����Խ�����ֵҪԽС����Ȼ���ܻ���Ҫ�ʵ����DacMinSpeechPow */
            T_U32   DacSpeechMultiple;
            //�Է��ֻ������ı�ʱ������ʱ�䣬����(ms)Ϊ��λ��Ĭ��ֵ��5000
            T_U32   DacConvergTime;

            //dac�������޻�������ֵ��Ĭ����0
            T_U16   DacFadeOutThreshold;  
            //DAC ����������á�����1024ʱ���Ŵ���=1��>1024ʱ���Ŵ���>1; <1024ʱ���Ŵ���<1
            T_U16   DacVolume;

            // for agc
            T_U8  m_PreprocessEna;
            T_U16 AGClevel;
            T_U16 maxGain;  // ���Ŵ���
            T_U16 minGain;  // ��С�Ŵ���
        }m_aec;
    }m_Private;
}T_AEC_IN_INFO;

typedef struct
{
    T_AEC_CB_FUNS	cb_fun;
    T_AEC_IN_INFO	m_info;
}T_AEC_INPUT;

typedef struct
{
    T_VOID *buf_near;  //near, mic
    T_U32  len_near;
    T_VOID *buf_far;   //far, speaker
    T_U32  len_far;
    T_VOID *buf_out; // output of aec
    T_U32  len_out;
}T_AEC_BUF;


/**
* @brief	open the AEC's device.
* @author	Tang Xuechai
* @date	    2013-07-15
* @param	[in] info: input information. Please refer "T_AEC_INPUT" for detail
* @return	T_VOID *
* @retval	the pointer to AEC's memory
*/
T_VOID *AECLib_Open(T_AEC_INPUT *info);

/**
* @brief	close the AEC's device.
* @author	Tang Xuechai
* @date	    2013-07-15
* @param	[in] p_aec: the pointer to AEC's memory, get from AECLib_Open
* @return	T_S32
* @retval	AK_TRUE :  close success
* @retval	AK_FLASE :  close fail
*/
T_S32  AECLib_Close(T_VOID *p_aec);

/**
* @brief	process ADC's data. Should be called in ADC's interrupt
* @author	Tang Xuechai
* @date	    2013-07-15
* @param	[in] p_aec: the pointer to AEC's memory, get from AECLib_Open
* @param	[in] in: ADC's pcm data
* @param	[in] len: ADC's pcm data length
* @return	T_VOID
*/
T_VOID AECLib_DacInt(T_VOID *p_aec, T_U8 *in, T_S32 len);

/**
* @brief	process DAC's data. Should be called in DAC's interrupt
* @author	Tang Xuechai
* @date	    2013-07-15
* @param	[in] p_aec: the pointer to AEC's memory, get from AECLib_Open
* @param	[in] in: DAC's pcm data
* @param	[in] len: DAC's pcm data length
* @return	T_VOID
*/
T_VOID AECLib_AdcInt(T_VOID *p_aec, T_U8 *in, T_S32 len);

/**
* @brief	main process of AEC. Should be called before transmit data to far end
* @author	Tang Xuechai
* @date	    2013-07-15
* @param	[in] p_aec: the pointer to AEC's memory, get from AECLib_Open
* @param	[in] p_aec_buf: buffer structure for saving output pcm data.
* @return	T_S32
* @retval	<0: error
* @retval	>=0: output pcm data length (bytes count)
*/
T_S32 AECLib_Control(T_VOID *p_aec, T_AEC_BUF *p_aec_buf);

/**
* @brief	pre process DAC/ADC data. Should be called while received fardata's packet
* @author	Tang Xuechai
* @date	    2013-08-13
* @param	[in] p_aec: the pointer to AEC's memory, get from AECLib_Open
* @param	[in] p_aec_buf: buffer structure for saving output pcm data.
* @return	T_S32
* @retval	<0: error
* @retval	>=0: output pcm data length (bytes count)
*/
T_S32 AECLib_FarPreprocess(T_VOID *p_aec, T_AEC_BUF *p_aec_buf);

/**
* @brief	set DAC's volume
* @author	Tang Xuechai
* @date	    2013-07-15
* @param	[in] p_aec: the pointer to AEC's memory, get from AECLib_Open
* @param	[in] volume: the object volume.
* @return	T_S32
* @retval	AK_TRUE :   success
* @retval	AK_FLASE :  fail
*/
T_S32 AECLib_SetDaVolume(T_VOID *p_aec, T_U16 volume);

#endif