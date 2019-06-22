#include "wavfile.h"

WavAudio loadWav(std::string const &path) {
  wave::File read_file;
  if (read_file.Open(path, wave::kIn)) {
    throw std::runtime_error("Something went wrong in in open");
  }
  std::vector<float> raw_data;
  if (read_file.Read(&raw_data)) {
    throw std::runtime_error("Something went wrong in read");
  }
  auto sample_rate = read_file.sample_rate();
  auto frame_count = read_file.frame_number();
  auto channel_count = read_file.channel_number();
  arma::fvec data(frame_count);
  for (int i = 0; i < frame_count; ++i) {
    float sum = 0;
    for (int j = 0; j < channel_count; ++j) {
      sum += raw_data[i * channel_count + j];
    }
    data[i] = sum / channel_count;
  }
  float length_sec = frame_count / sample_rate;
  auto bits_per_sample = read_file.bits_per_sample();
  return {sample_rate, frame_count, bits_per_sample, length_sec, data};
}

void saveWav(WavAudio const &audio, std::string const &path) {
  wave::File write_file;
  if (write_file.Open("/home/asset/test_write.wav", wave::kOut)) {
    throw std::runtime_error("Something went wrong in out open");
  }
  write_file.set_sample_rate(audio.sampling_rate);
  write_file.set_bits_per_sample(audio.bits_per_sample);
  write_file.set_channel_number(1);
  auto s = audio.samples;
  if (write_file.Write(std::vector<float>(s.begin(), s.end()))) {
    throw std::runtime_error("Something went wrong in write");
  }
}