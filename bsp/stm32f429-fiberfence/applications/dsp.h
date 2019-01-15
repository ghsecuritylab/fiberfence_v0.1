#ifndef DSP_H_
#define DSP_H_
#include <stm32f4xx.h>
#include <rtthread.h>
#include <arm_math.h>

#define NUM_STAGES   4

const float32_t scaleValue[NUM_STAGES]={0.41837841942899801,                                                   
																				0.32142553861385142,                                                   
																				0.2730048026138932,                                                    
																				0.25242513913381587                                                   
};

const float32_t coefs[5*NUM_STAGES]={1,  2,  1,  0.00000000000000018579734170254186,  -0.67351367771599213,
																		 1,  2,  1,  0.00000000000000014274161346865612,  -0.28570215445540592, 
																		 1,  2,  1,  0.00000000000000012123848707807617,  -0.092019210455573028,
																		 1,  2,  1,  0.00000000000000011209928058422843,  -0.0097005565352636346
};

float32_t State[4*NUM_STAGES];

void arm_iir_f32_init(arm_biquad_casd_df1_inst_f32 *S, uint16_t nS, float32_t * coefs, float32_t *State);

void arm_iir_f32_lp(arm_biquad_casd_df1_inst_f32 *S, float32_t * in, float32_t *out, uint16_t blockSize, float32_t *scale);

void median_filter(uint16_t *in, uint16_t *out, uint16_t dlen, uint16_t *s, uint16_t slen, uint16_t tmp);

#endif
