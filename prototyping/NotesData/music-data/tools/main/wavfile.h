#pragma once
#include <armadillo>
#include <string>
#include <wave/file.h>

struct WavAudio {
  size_t const sampling_rate;
  size_t const data_count;
  size_t const bits_per_sample;
  float const length_sec;
  arma::fvec const samples;
};

WavAudio loadWav(std::string const &path);
void saveWav(WavAudio const &audio, std::string const &path);