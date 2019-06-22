#pragma once

#include <armadillo>

#include "wavfile.h"

struct FrameAudio {
  arma::fmat frames;
  arma::fvec timeSeq;
  size_t hop_width;
  size_t frame_count;
};

struct Spectrogram {
  arma::cx_fmat spectrogram;
  arma::fvec timeSeq;
  arma::fvec freqSeq;
  size_t hop_width;
  size_t frame_count;
};

FrameAudio makeFrames(WavAudio const &samples, size_t hop_width,
                      size_t frame_width);

arma::fvec makeTimeSeq(size_t sample_count, size_t sampling_rate,
                       size_t hop_width);

arma::fvec makeFreqSeq(WavAudio const &wav, size_t hop_width,
                       size_t frame_width);
