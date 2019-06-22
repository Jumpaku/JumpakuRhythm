#include "analysis.h"
#include <algorithm>

size_t compute_frame_count(size_t sample_count, size_t hop_width) {
  return static_cast<size_t>(ceil(sample_count / hop_width));
}

FrameAudio makeFrames(WavAudio const &wav, size_t hop_width,
                      size_t frame_width) {
  auto const &samples = wav.samples;
  size_t sample_count = samples.size();
  auto frame_count = compute_frame_count(sample_count, hop_width);
  arma::fmat X = arma::zeros<arma::fmat>(frame_width, frame_count);
  for (size_t i = 0; i < frame_count; ++i) {
    auto begin = i * hop_width;
    auto end = std::min(begin + frame_width, sample_count);
    X.submat(0, i, end - begin - 1, i) = samples.subvec(begin, end - 1);
  }
  return {
      X,
      makeTimeSeq(sample_count, wav.sampling_rate, hop_width),
      hop_width,
      frame_count,
  };
}

arma::fvec makeTimeSeq(size_t sample_count, size_t sampling_rate,
                       size_t hop_width) {
  auto frame_count = compute_frame_count(sample_count, hop_width);
  auto last_time =
      ((frame_count - 1) * hop_width) / static_cast<float>(sampling_rate);
  return arma::linspace<arma::fvec>(0.0f, last_time, frame_count);
}
