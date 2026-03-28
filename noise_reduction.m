
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



% Find the next power of 2 for FFT computation
NFFT = 2^nextpow2(frameLen); 

% Pre-allocate memory for magnitude and phase matrices
% Rows = frequency bins, Columns = individual frames
magFrames = zeros(NFFT, numFrames);
phaseFrames = zeros(NFFT, numFrames);


for i = 1:numFrames
    % Compute the FFT of the current windowed frame.
    complexSpectrum = fft(windowedFrames(:, i), NFFT);
    
    magFrames(:, i) = abs(complexSpectrum);
    phaseFrames(:, i) = angle(complexSpectrum);
end


% Initialize the noise profile with the very first frame (our best initial guess)
adaptiveNoiseProfile = magFrames(:, 1);

% Smoothing factor for the leaky integrator (typically between 0.90 and 0.99)
% Higher = slower, smoother updates. Lower = reacts faster to changing noise.
alpha_smooth = 0.95; 

% VAD Threshold multiplier
% If a frame's energy is less than this multiplier times the noise energy,
% we assume it is just noise and update our profile.
vadThreshold = 2.0; 


cleanMagFrames = zeros(NFFT, numFrames);

% The Adaptive Subtraction Loop with Flooring
for i = 1:numFrames
    currentMag = magFrames(:, i);
    
    % Calculate the total energy of the current frame and the current noise profile
    frameEnergy = sum(currentMag.^2);
    noiseEnergy = sum(adaptiveNoiseProfile.^2);
    
    % Voice Activity Detection (VAD)
    if frameEnergy < (vadThreshold * noiseEnergy)
        % No speech detected. Update the background noise estimate.
        adaptiveNoiseProfile = (alpha_smooth * adaptiveNoiseProfile) + ((1 - alpha_smooth) * currentMag);
    end
    
    % Spectral Subtraction with Flooring
    % Tune these two parameters to eliminate "musical noise"
    alpha_overSub = 3.0;
    beta_floor = 0.1;  
    
    % Calculate the aggressive subtraction
    subtractedMag = currentMag - (alpha_overSub * adaptiveNoiseProfile);
    
    % Calculate the minimum allowed noise floor
    spectralFloor = beta_floor * adaptiveNoiseProfile;
    
    
    cleanMag = max(subtractedMag, spectralFloor);
    
    % Store the cleaned magnitude
    cleanMagFrames(:, i) = cleanMag;
end



% Calculate the total length of the final reconstructed audio array
outputAudioLen = (numFrames - 1) * stepSize + frameLen;

% Initialize the output buffer with zeros. 
outputAudio = zeros(outputAudioLen, 1);


for i = 1:numFrames
    % Get the cleaned magnitude and original phase for the current frame
    cleanMag = cleanMagFrames(:, i);
    origPhase = phaseFrames(:, i);
    
    % Recombine into a complex spectrum
    complexFrame = cleanMag .* exp(1i * origPhase);
    
    % Compute Inverse FFT to transform back to the time domain
    timeFrame = ifft(complexFrame, NFFT);
    
    % Extract the real part to discard tiny floating-point rounding errors
    timeFrameReal = real(timeFrame(1:frameLen));
    
    
    % Calculate the exact indices in the global output buffer
    startIndex = (i - 1) * stepSize + 1;
    endIndex = startIndex + frameLen - 1;
    
    % Add the current reconstructed frame to whatever is already in the buffer
    outputAudio(startIndex:endIndex) = outputAudio(startIndex:endIndex) + timeFrameReal;
end


% Normalize the output to fall within the standard [-1.0, 1.0] audio range 
% to prevent digital clipping when playing or saving the file.
maxAmp = max(abs(outputAudio));
if maxAmp > 0
    outputAudio = outputAudio / maxAmp;
end


% Save the audio
audiowrite('cleaned_output.wav', outputAudio, Fs);