#ifndef NOISE_REDUCER_H
#define NOISE_REDUCER_H

#include <stdint.h>
#include <math.h>
#include "dsps_fft2r.h"

#define FRAME_LEN 512
#define NFFT 512
#define OVERLAP_LEN 256
#define STEP_SIZE 256

typedef struct
{
    float alpha_overSub;
    float beta_floor;
    float alpha_smooth;
    float vadThreshold;

    // ESP32 DSP requires a specific buffer alignment
    float __attribute__((aligned(16))) fft_buffer[NFFT * 2];
    float mag_buffer[NFFT / 2];
    float noise_profile[NFFT / 2];
    float window[FRAME_LEN];
} NoiseReducer_State;

#ifdef __cplusplus
extern "C"
{
#endif

    void NoiseReducer_Init(NoiseReducer_State *state);
    void NoiseReducer_ProcessFrame(NoiseReducer_State *state, float *inputFrame, float *outputFrame);

#ifdef __cplusplus
}
#endif

#endif