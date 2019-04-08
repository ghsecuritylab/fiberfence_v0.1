#include "dsp.h"

//#define numStages 2 /* 2阶IIR滤波器个数 */
//#define TEST_LENGTH_SAMPLES 400 /* 采样点数 */

//static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* 采样点 */
//static float32_t testOutput[TEST_LENGTH_SAMPLES]; /* 滤波后输出 */
//static float32_t IIRStateF32[4*numStages]; /* 状态缓存，大小numTaps + blockSize - 1*/
///* 巴特沃斯低通滤波器系数 80Hz*/
//const float32_t IIRCoeffs32LP[5*numStages] = {
//	1.0f, 2.0f, 1.0f, 1.4797988943972167f, -0.68867695305386178f,
//	1.0f, 2.0f, 1.0f, 1.2128120926202184f, -0.38400416228655354f
//};
///*
//*********************************************************************************************************
//* 函 数 名: arm_iir_f32_lp
//* 功能说明: 调用函数arm_iir_f32_lp实现低通滤波
//* 形    参: 无
//* 返回参数: 无
//*********************************************************************************************************
//*/
//static void arm_iir_f32_lp1(void)
//{
//	uint32_t i;
//	arm_biquad_casd_df1_inst_f32 S;
//	float32_t ScaleValue;
//	/* 初始化 */
//	arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t *)&IIRCoeffs32LP[0], (float32_t	*)&IIRStateF32[0]);
//	/* IIR滤波 */
//	arm_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput, TEST_LENGTH_SAMPLES);
//	/* 缩放系数 */
//	ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
//	/* 打印滤波结果 */
//	for(i=0; i<TEST_LENGTH_SAMPLES; i++)
//	{
//		printf("%f\r\n", testOutput[i]*ScaleValue);
//	}
//}


//void arm_iir_f32_init(arm_biquad_casd_df1_inst_f32 *S, uint16_t nS, float32_t * coefs, float32_t *State)
//{
//	arm_biquad_cascade_df1_init_f32(S, nS, coefs, State);
//}

//void arm_iir_f32_lp(arm_biquad_casd_df1_inst_f32 *S, float32_t * in, float32_t *out, uint16_t blockSize, float32_t *scale)
//{
//	uint16_t i,j;
//	float32_t tmp;
//	arm_biquad_cascade_df1_f32(S, in, out, blockSize);
//	for(i=0; i<blockSize; i++){
//		for(j=0; j<S->numStages; j++)
//			tmp*=scale[j];
//		out[i]*=tmp;
//	}
//}


void median_filter(uint16_t *in, uint16_t *out, uint16_t dlen, uint16_t *s, uint16_t slen, float32_t *tmp)
{
	int i;
	float32_t temp=*tmp;
	for(i=0;i<slen;i++)
	{
		temp = temp-s[i]+in[i];
		out[i] = ((float)temp)/slen;
	}
	
	for(i=slen; i<dlen; i++)
	{
		temp = temp-in[i-slen]+in[i];
		out[i] = ((float)temp)/slen;
	}
	
	for(i=0;i<slen;i++)
	{
		s[i] = in[dlen-slen+i];
	}
	*tmp=temp;
}
