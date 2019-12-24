#include "analysis.h"
#include <algorithm>

using namespace arma;

size_t compute_frame_count(size_t sample_count, size_t hop_width) {
  return static_cast<size_t>(ceilf(sample_count * 1.0f / hop_width));
}

FrameAudio makeFrames(WavAudio const &wav, size_t hop_width,
                      size_t frame_width) {
  fvec const &samples = wav.samples;
  size_t sample_count = samples.size();
  auto frame_count = compute_frame_count(sample_count, hop_width);
  fmat X = zeros<fmat>(frame_width, frame_count);
  for (size_t i = 0; i < frame_count; ++i) {
    auto begin = i * hop_width;
    auto end = std::min(begin + frame_width, sample_count);
    X.submat(0, i, end - begin - 1, i) = samples.subvec(begin, end - 1);
  }
  auto sampling_rate = wav.sampling_rate;
  return {X, makeTimeSeq(sample_count, sampling_rate, hop_width), hop_width,
          sampling_rate};
}

Spectrogram FFT(FrameAudio const &audio) {
  fmat const &X = audio.frames;
  cx_fmat &&Y = fft(X);
  return {Y, audio.timeSeq,
          makeFreqSeq(X.n_cols, audio.sampling_rate, audio.hop_width),
          audio.hop_width, audio.sampling_rate};
}

fvec makeTimeSeq(size_t sample_count, size_t sampling_rate, size_t hop_width) {
  auto frame_count = compute_frame_count(sample_count, hop_width);
  auto last_time =
      ((frame_count - 1) * hop_width) / static_cast<float>(sampling_rate);
  return linspace<fvec>(0.0f, last_time, frame_count);
}

fvec makeFreqSeq(size_t sample_count, size_t sampling_rate, size_t hop_width) {
  auto bin_count = compute_frame_count(sample_count, hop_width);
  auto mid = bin_count / 2 + 1;
  auto fs = static_cast<float>(sampling_rate);
  fvec freq(bin_count);
  freq.subvec(0, mid - 1) =
      linspace<fvec>(0.0f, (mid - 1) * fs / bin_count, mid);
  freq.subvec(mid, bin_count - 1) =
      (bin_count - mid == 1)
          ? fvec{-fs / bin_count}
          : linspace<fvec>((mid - bin_count) * fs / bin_count, -fs / bin_count,
                           mid);
  return freq;
}
