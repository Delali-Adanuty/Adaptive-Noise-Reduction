#include "NoiseReducer.h"
#include "dsps_view.h"

void NoiseReducer_Init(NoiseReducer_State *state)
{
    state->alpha_overSub = 3.0f;
    state->beta_floor = 0.1f;
    state->alpha_smooth = 0.95f;
    state->vadThreshold = 2.0f;

    // 1. Initialize the FFT lookup tables
    dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);

    // 2. Pre-calculate Hann window
    dsps_wind_hann_f32(state->window, FRAME_LEN);

    // 3. Clear buffers
    for (int i = 0; i < NFFT / 2; i++)
        state->noise_profile[i] = 0;
}

void NoiseReducer_ProcessFrame(NoiseReducer_State *state, float *input, float *output)
{

    for (int i = 0; i < FRAME_LEN; i++)
    {
        state->fft_buffer[i * 2] = input[i] * state->window[i];
        state->fft_buffer[i * 2 + 1] = 0;
    }

    // 2. Execute ESP32 Native FFT
    dsps_fft2r_fc32(state->fft_buffer, NFFT);
    dsps_bit_rev_fc32(state->fft_buffer, NFFT);

    // 3. Simple Magnitude and Subtraction logic
    for (int i = 0; i < NFFT / 2; i++)
    {
        float real = state->fft_buffer[i * 2];
        float imag = state->fft_buffer[i * 2 + 1];
        float mag = sqrtf(real * real + imag * imag);

        // Adaptive Noise Tracking (VAD simplified)
        if (mag < state->noise_profile[i] * state->vadThreshold)
        {
            state->noise_profile[i] = state->noise_profile[i] * state->alpha_smooth + mag * (1 - state->alpha_smooth);
        }

        // Spectral Subtraction
        float cleanMag = mag - (state->alpha_overSub * state->noise_profile[i]);
        if (cleanMag < state->beta_floor * state->noise_profile[i])
            cleanMag = state->beta_floor * state->noise_profile[i];

        // Apply Gain
        float gain = (mag > 0.00001f) ? (cleanMag / mag) : 0;
        state->fft_buffer[i * 2] *= gain;
        state->fft_buffer[i * 2 + 1] *= gain;
    }

    // 4. Inverse FFT
    dsps_ifft2r_fc32(state->fft_buffer, NFFT);
    dsps_bit_rev_fc32(state->fft_buffer, NFFT);

    for (int i = 0; i < FRAME_LEN; i++)
    {
        output[i] = state->fft_buffer[i * 2];
    }
}