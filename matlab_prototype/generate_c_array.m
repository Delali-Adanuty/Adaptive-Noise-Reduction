fileName = 'noisy_speech.wav'; 
[inputAudio, Fs] = audioread(fileName);

% 1. Convert stereo to mono if necessary
if size(inputAudio, 2) > 1
    inputAudio = mean(inputAudio, 2);
end



targetDuration = 1.0; 
numSamples = min(length(inputAudio), floor(Fs * targetDuration));
inputAudio = inputAudio(1:numSamples);


fileID = fopen('audio_test_array.h', 'w');

% Write the C header guards and metadata
fprintf(fileID, '#ifndef AUDIO_TEST_ARRAY_H\n#define AUDIO_TEST_ARRAY_H\n\n');
fprintf(fileID, '// Auto-generated from %s\n', fileName);
fprintf(fileID, '#define AUDIO_SAMPLE_RATE %d\n', Fs);
fprintf(fileID, '#define AUDIO_LENGTH %d\n\n', length(inputAudio));


fprintf(fileID, 'const float test_audio[] = {\n');

% Write the floating-point audio data
fprintf(fileID, '    %.6f,\n', inputAudio(1:end-1));
fprintf(fileID, '    %.6f\n', inputAudio(end)); 

fprintf(fileID, '};\n\n#endif\n');
fclose(fileID);