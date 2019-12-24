import numpy as np
import librosa
import librosa.display as ld
import matplotlib
matplotlib.use('Agg')
import matplotlib.pylab as plt
from matplotlib.backends.backend_pdf import PdfPages 

y, sr = librosa.load('../asset/2019-05-23_0753.wav', sr=None)

y_harmonic, y_percussive = librosa.effects.hpss(y)
times = librosa.frames_to_time(np.arange(0, len(y) // 512 + 1), sr=sr)

librosa.output.write_wav('../asset/y_harmonic.wav', y_harmonic, sr)
librosa.output.write_wav('../asset/y_percussive.wav', y_percussive, sr)
envelope_harmonic = librosa.onset.onset_strength(y_harmonic, sr=sr)
onsets_harmonic = librosa.onset.onset_detect(onset_envelope=envelope_harmonic, sr=sr)
envelope_percussive = librosa.onset.onset_strength(y_percussive, sr=sr)
onsets_percussive= librosa.onset.onset_detect(onset_envelope=envelope_percussive, sr=sr)

D = librosa.stft(y)
D_harmonic, D_percussive = librosa.decompose.hpss(D)
rp = np.max(np.abs(D))

"""
fig = plt.figure(figsize=(100,10))
plt.subplot(2, 1, 1)
librosa.display.specshow(librosa.amplitude_to_db(np.abs(D_harmonic), ref=np.max), y_axis='log', sr=sr)
plt.title('Power spectrogram')
onset_subbands_hermonic = librosa.onset.onset_strength_multi(y=y_harmonic, sr=sr, channels=[0,1,2,3,5,8,13,21,34,56,90,128])
plt.subplot(2, 1, 2)
librosa.display.specshow(onset_subbands_hermonic, x_axis='time')
plt.ylabel('Sub-bands')
plt.title('Sub-band onset strength')

pp = PdfPages('onset_multi.pdf')
pp.savefig(fig)
pp.close()
"""

fig = plt.figure(figsize=(100,10))
ax1 = plt.subplot(7, 1, 1)
plt.plot(times, envelope_percussive, label='Onset strength')
plt.plot(times, np.repeat(1760, len(times)))
plt.plot(times, np.repeat(110, len(times)))
librosa.display.specshow(librosa.amplitude_to_db(np.abs(D), ref=np.max), x_axis='time', y_axis='log', sr=sr)
plt.title('Power spectrogram')
plt.subplot(7, 1, 2, sharex=ax1)
librosa.display.specshow(librosa.amplitude_to_db(np.abs(D_harmonic), ref=rp), x_axis='time', y_axis='log', sr=sr)
plt.subplot(7, 1, 3, sharex=ax1)
plt.plot(times, envelope_harmonic, label='Onset strength')
plt.vlines(times[onsets_harmonic], 0, envelope_harmonic.max(), color='r', alpha=0.9, linestyle='--', label='Onsets')
plt.axis('tight')
plt.legend(frameon=True, framealpha=0.75)
plt.subplot(7, 1, 4, sharex=ax1)
librosa.display.specshow(librosa.amplitude_to_db(np.abs(D_percussive), ref=rp), x_axis='time', y_axis='log', sr=sr)
plt.subplot(7, 1, 5, sharex=ax1)
plt.plot(times, envelope_percussive, label='Onset strength')
plt.vlines(times[onsets_percussive], 0, envelope_percussive.max(), color='r', alpha=0.9, linestyle='--', label='Onsets')
plt.axis('tight')
plt.legend(frameon=True, framealpha=0.75)

pp = PdfPages('onset.pdf')
pp.savefig(fig)
pp.close()