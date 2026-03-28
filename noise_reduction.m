
fileName = 'noisy_speech.wav'; 
[inputAudio, Fs] = audioread(fileName); 

% Convert stereo to mono if necessary
if size(inputAudio, 2) > 1
    inputAudio = mean(inputAudio, 2);
end

% Framing Parameters
frameDurationMs = 25; % 25 ms frame size
overlapPercent = 0.5; % 50% overlap

% Convert time (ms) to discrete samples based on the sampling rate (Fs)
frameLen = floor((frameDurationMs / 1000) * Fs);
overlapLen = floor(frameLen * overlapPercent);
stepSize = frameLen - overlapLen; 


% A Hamming or Hann window tapers the edges of the frame to zero
win = hamming(frameLen); 

% Calculate the total number of complete frames we can extract
totalSamples = length(inputAudio);
numFrames = floor((totalSamples - overlapLen) / stepSize);

% Pre-allocate memory for the framed data
% (This mirrors a 2D array or buffer pool)
windowedFrames = zeros(frameLen, numFrames);


% The Framing & Windowing Loop 
for i = 1:numFrames
    
    startIndex = (i - 1) * stepSize + 1;
    endIndex = startIndex + frameLen - 1;
    
    % Extract the raw audio chunk
    rawFrame = inputAudio(startIndex:endIndex);
    
    % Apply the window function
    windowedFrames(:, i) = rawFrame .* win;
end

disp(['Successfully extracted ', num2str(numFrames), ' windowed frames.']);