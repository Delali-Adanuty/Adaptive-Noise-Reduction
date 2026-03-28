% --- MATLAB to C-Array Converter ---
fileName = 'noisy_speech.wav'; 
[inputAudio, Fs] = audioread(fileName);

% Convert stereo to mono if necessary
if size(inputAudio, 2) > 1
    inputAudio = mean(inputAudio, 2);
end



% Open a new C header file for writing
fileID = fopen('audio_test_array.h', 'w');

% Write the C header guards and metadata
fprintf(fileID, '#ifndef AUDIO_TEST_ARRAY_H\n#define AUDIO_TEST_ARRAY_H\n\n');
fprintf(fileID, '// Auto-generated from %s\n', fileName);
fprintf(fileID, 'const int AUDIO_SAMPLE_RATE = %d;\n', Fs);
fprintf(fileID, 'const int AUDIO_LENGTH = %d;\n\n', length(inputAudio));

% Write the array declaration
fprintf(fileID, 'const float test_audio[] = {\n');

% Write the floating-point audio data into the array
fprintf(fileID, '    %f,\n', inputAudio(1:end-1));
fprintf(fileID, '    %f\n', inputAudio(end)); % Last element without a trailing comma

% Close the array and the file
fprintf(fileID, '};\n\n#endif\n');
fclose(fileID);
