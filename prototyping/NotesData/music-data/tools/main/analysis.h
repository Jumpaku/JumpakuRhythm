#pragma once

#include <armadillo>

#include "wavfile.h"

struct FrameAudio {
  arma::fmat const frames;
  arma::fvec const timeSeq;
  size_t const hop_width;
  size_t const sampling_rate;
};

struct Spectrogram {
  arma::cx_fmat const spectrogram;
  arma::fvec const timeSeq;
  arma::fvec const freqSeq;
  size_t const hop_width;
  size_t const sampling_rate;
};

FrameAudio makeFrames(WavAudio const &samples, size_t hop_width,
                      size_t frame_width);

arma::fvec makeTimeSeq(size_t sample_count, size_t sampling_rate,
                       size_t hop_width);

// Spectrogram FFT(FrameAudio const &audio);

arma::fvec makeFreqSeq(size_t sample_count, size_t sampling_rate,
                       size_t hop_width);
