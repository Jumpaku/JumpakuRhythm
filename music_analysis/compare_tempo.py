from librosa.core import load
from librosa.beat import beat_track
from librosa.onset import onset_detect
from matplotlib import pyplot as plt
from librosa import stft, amplitude_to_db
from librosa.display import specshow
import numpy as np
import bisect


def kernelDensity(x, h, xs):
    def kernel(x): return 0 if abs(x) >= 1 else 15*(1-x*x)*(1-x*x)/16
    i = bisect.bisect_left(xs, x)
    begin = i
    while begin > 0 and abs((x-xs[begin-1])/h) < 1:
        begin -= 1
    end = i+1
    while end < len(xs) and abs((x-xs[end-1])/h) < 1:
        end += 1
    return sum([kernel((x-xi)/h) for xi in xs[begin:end]])/(len(xs)*h)


#path = "asset/2019-05-23_0753.wav"
path = "asset/crow_song/('crow_song', '.mp3')/other.wav"
window_size = 1024
hop_size = window_size//2

y, sample_rate = load(path, sr=44100)
print(path, sample_rate)
tempo, beats = beat_track(
    y=y, sr=sample_rate, hop_length=hop_size, units='time')
print(tempo)
avg = np.average(np.diff(beats, n=1))
print(avg)


onset = onset_detect(y=y, sr=sample_rate, hop_length=hop_size, units='time')

D = amplitude_to_db(
    np.abs(stft(y=y, n_fft=window_size, hop_length=hop_size)), ref=np.max)

fig = plt.figure(figsize=(200, 20))
specshow(D, sr=sample_rate, x_axis='s', y_axis='log', cmap='gray_r')
plt.scatter(onset, [0]*len(onset), c='orange')
for beat in beats:
    plt.axvline(beat, c='orange')
fig.savefig("onset_with_tracked.pdf")


diffs = np.diff(onset, n=1)
diffs.sort()
t = np.arange(0, diffs[-1], 0.01)
d = np.frompyfunc(lambda x: kernelDensity(x, 0.01, diffs), 1, 1)(t)
fig = plt.figure()
plt.plot(t, d)
fig.savefig("width.pdf")
