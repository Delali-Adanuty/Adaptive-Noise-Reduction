import numpy as np
from scipy.io import wavfile

def add_noise_to_wav(input_path, output_path, target_snr_db):
    # Load the clean speech
    sample_rate, signal = wavfile.read(input_path)
    
    # Convert to float for math operations; handle stereo by taking one channel if needed
    signal = np.array(signal, dtype=float)
    if len(signal.shape) > 1:
        signal = signal[:, 0]

    # Calculate signal power
    signal_power = np.mean(signal ** 2)

    # Calculate required noise power based on target SNR
    noise_power = signal_power / (10 ** (target_snr_db / 10))

    # Generate Gaussian white noise
    noise = np.random.normal(0, np.sqrt(noise_power), signal.shape)

    # Mix the signal and the noise
    noisy_signal = signal + noise

    # Normalize to prevent clipping and cast back to 16-bit PCM
    max_val = np.max(np.abs(noisy_signal))
    if max_val > 0:
        noisy_signal = (noisy_signal / max_val) * 32767
        
    noisy_signal = np.int16(noisy_signal)

    # Export the new wav file
    wavfile.write(output_path, sample_rate, noisy_signal)

# Add noise at a 10dB SNR
add_noise_to_wav('clean_speech.wav', 'noisy_speech.wav', 10)