#include "NoiseReducer.h"

// --- Initialization ---
void NoiseReducer_Init(NoiseReducer_State *state)
{
    // 1. Set the DSP Tuning Parameters
    state->alpha_overSub = 3.0f;
    state->beta_floor = 0.1f;
    state->alpha_smooth = 0.95f;
    state->vadThreshold = 2.0f;

    // 2. Clear the Adaptive Noise Profile
    for (int i = 0; i < NFFT_HALF; i++)
    {
        state->adaptiveNoiseProfile[i] = 0.0f;
    }

    // 3. Clear the Overlap-Add Buffer
    for (int i = 0; i < OVERLAP_LEN; i++)
    {
        state->previousOverlapBuffer[i] = 0.0f;
    }
}

// --- Main Processing Loop (Skeleton) ---
void NoiseReducer_ProcessFrame(NoiseReducer_State *state, const float *inputFrame, float *outputFrame)
{
    // 1. Apply Windowing (Hamming)
    // 2. Compute FFT
    // 3. VAD & Energy Tracking
    // 4. Spectral Subtraction with Flooring
    // 5. Compute Inverse FFT
    // 6. Overlap-Add (OLA) Reconstruction

    // For now, as a passthrough test, we just copy input to output
    for (int i = 0; i < FRAME_LEN; i++)
    {
        outputFrame[i] = inputFrame[i];
    }
}