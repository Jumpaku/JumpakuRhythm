import matplotlib.pyplot as plt
import sys
import math
import numpy
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


def widthCandidates(onsets, binsize, bandwidth, maxResultSize=5):
    diffs = [onsets[i+1] - onsets[i] for i in range(len(onsets)-1)]
    diffs.sort()
    x = numpy.arange(0, diffs[-1], binsize)
    y = [kernelDensity(xi, bandwidth, diffs) for xi in x]

    def evaluate(T, begin, end, f):
        x = begin
        prob = 0
        while x <= end:
            prob += f(x)
            x += T
        return prob * T/(end-begin)*binsize
    e = [0]+[evaluate(xi, x[0], x[-1], lambda t: kernelDensity(t, bandwidth, diffs))
             for xi in x[1:]]
    values = list(zip(x, e))
    l = len(values)
    peaks = []
    for i in range(l):
        p = 0 if i == 0 else values[i-1][1]
        c = values[i][1]
        n = 0 if i == l-1 else values[i+1][1]
        if c > p and c > n:
            peaks.append(values[i])
    peaks.sort(key=lambda xy: xy[1], reverse=True)
    """"""
    fig = plt.figure()
    plt.xticks(numpy.arange(x[0], x[-1], 0.1))
    plt.tick_params(labelsize=5)
    plt.plot(x, y)
    plt.plot(x, [ei/binsize for ei in e])
    fig.savefig("width.pdf")
    """"""
    return peaks[:min(len(peaks), maxResultSize)]


def selectWidth(candidates, tempo):
    def penalty(w, tempo):
        diffs = [a-b for a, b in zip(tempo[1:], tempo)]
        result = sum([(d/w - round(d/w))**2 for d in diffs])/len(diffs)
        return result
    selected = min(candidates, key=lambda w: penalty(w[0], tempo))
    return selected[0]


def separateTempo(tempo, width):
    separaters = tempo.copy()
    for s0, s1 in zip(tempo, tempo[1:]):
        n = int(round(abs((s0 - s1)/width)))
        if n > 1:
            separaters += [(n-i) * s0 / n + i * s1 / n for i in range(1, n)]
    separaters.sort()
    return separaters


def separateBlock(tempo, onset, binsize, bandwidth):
    candidates = widthCandidates(onset, binsize, bandwidth, 5)
    width = selectWidth(candidates, tempo)
    frameSeps = separateTempo(tempo, width)
    avg = sum([a-b for a, b in zip(frameSeps[1:], frameSeps)]) / \
        (len(frameSeps)-1)
    print(width)
    print(candidates)
    print(avg)
    return frameSeps


onset = []
tempo = []
label = ""
duration = 0
sample_rate = 0
for l in [str(it).strip() for it in sys.stdin]:
    if l in {"onset", "tempo", "duration", "sample_rate"}:
        label = l
        continue
    if label == "onset":
        onset.append(float(l))
    elif label == "tempo":
        tempo.append(float(l))
    elif label == "duration":
        duration = float(l)
    elif label == "sample_rate":
        sample_rate = float(l)
print("duration =", duration)
print("sample_rate =", sample_rate)

onset.sort()
tempo.sort()

binsize = 0.01
bandwidth = 0.01
separaters = separateBlock(tempo, onset, binsize, bandwidth)

fig = plt.figure(figsize=(500, 5))
plt.xticks(numpy.arange(0, 125, 0.1))
plt.tick_params(labelsize=5)
plt.scatter(onset, [0]*len(onset))
plt.vlines(tempo, [0]*len(tempo), [1]*len(tempo))
plt.vlines(separaters, [0]*len(separaters), [0.2]*len(separaters))
fig.savefig("onset_with_tracked.pdf")

"""
onset_data=[float(l) for l in sys.stdin]
onset_data.sort()
width=computeWidth(onset_data, binsize, band)
print(width)

t=numpy.arange(0, onset_data[-1], binsize)
y=[0]*len(t)
j=0
for i in range(len(t)-1):
    for o in onset_data[j:]:
        if t[i] <= o and o < t[i+1]:
            y[i]=1
            j += 1
            break
start=computeStart(t, y, width)
print(start)
"""
