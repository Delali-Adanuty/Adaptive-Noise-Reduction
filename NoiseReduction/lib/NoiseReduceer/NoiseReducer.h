#ifndef NOISE_REDUCER_H
#define NOISE_REDUCER_H

#include <stdint.h>
#include <math.h>

// Define our buffer sizes based on a 16kHz sample rate and 25ms frames
// (Adjust these if your test .wav file uses a different sample rate)
#define FRAME_LEN 400   // 25ms at 16kHz = 400 samples
#define OVERLAP_LEN 200 // 50% overlap
#define STEP_SIZE 200   // FRAME_LEN - OVERLAP_LEN
#define NFFT 512        // Next power of 2 above 400
#define NFFT_HALF 256   // We only process the first half of the FFT spectrum

// The State Struct
// This holds all the variables that need to survive between audio frames
typedef struct
{
    float adaptiveNoiseProfile[NFFT_HALF];
    float previousOverlapBuffer[OVERLAP_LEN];

    // Tuning Parameters
    float alpha_overSub;
    float beta_floor;
    float alpha_smooth;
    float vadThreshold;
} NoiseReducer_State;

// Function Prototypes
void NoiseReducer_Init(NoiseReducer_State *state);
void NoiseReducer_ProcessFrame(NoiseReducer_State *state, const float *inputFrame, float *outputFrame);

#endif // NOISE_REDUCER_H