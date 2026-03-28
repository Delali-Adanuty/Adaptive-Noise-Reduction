#include <stdio.h>
#include "audio_test_array.h"

int main()
{
  printf("\n--- Native DSP Environment Test ---\n");

  // Check if the metadata came through
  printf("Audio Sample Rate: %d Hz\n", AUDIO_SAMPLE_RATE);
  printf("Total Audio Samples: %d\n", AUDIO_LENGTH);

  // Print the first 5 samples to prove the array is accessible
  printf("\nFirst 5 audio samples:\n");
  for (int i = 0; i < 5; i++)
  {
    printf("Sample [%d]: %f\n", i, test_audio[i]);
  }

  printf("\nEnvironment setup complete. Ready for math!\n\n");

  return 0;
}