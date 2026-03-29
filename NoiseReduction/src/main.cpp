#include <stdio.h>
#include <Arduino.h>
#include "NoiseReducer.h"
#include "audio_test_array.h"

float output_audio[AUDIO_LENGTH] = {0};

int main()
{
    printf("\n--- Starting DSP Native Simulation ---\n");

    NoiseReducer_State dspState;
    NoiseReducer_Init(&dspState);

    int numFrames = (AUDIO_LENGTH - OVERLAP_LEN) / STEP_SIZE;
    printf("Total samples: %d\n", AUDIO_LENGTH);
    printf("Processing %d frames...\n", numFrames);

    for (int i = 0; i < numFrames; i++)
    {

        int startIndex = i * STEP_SIZE;

        float inputFrame[FRAME_LEN];
        for (int j = 0; j < FRAME_LEN; j++)
        {
            inputFrame[j] = test_audio[startIndex + j];
        }

        // Execute DSP Algorithm
        float outputFrame[FRAME_LEN] = {0};
        NoiseReducer_ProcessFrame(&dspState, inputFrame, outputFrame);

        // Add the processed frame into the global output buffer
        for (int j = 0; j < FRAME_LEN; j++)
        {
            output_audio[startIndex + j] += outputFrame[j];
        }
    }

    printf("\nProcessing complete!\n");

    // Print a few output samples to prove the loop executed
    printf("First 5 output samples:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("Sample [%d]: %f\n", i, output_audio[i]);
    }
    printf("\n");

    return 0;
}

void setup()
{
    // Initialize things here
}

void loop()
{
    // Your main code here
}