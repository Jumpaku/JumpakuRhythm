#include <iostream>

#include "analysis.h"
#include "wavfile.h"

using namespace std;
using namespace arma;

int main(int argc, char **argv) {
  // read file's content
  WavAudio &&w = loadWav("/home/asset/2019-05-23_0753.wav");
  auto data = w.samples;
  auto hop_width = 1024;
  auto frame_width = 2048;
  auto frame_count = (w.data_count - frame_width) / hop_width;
  auto &&frames = makeFrames(w, hop_width, frame_width);

  cout << frames.frame_count << "," << frames.frames.n_rows << ","
       << frames.frames.n_cols << "," << frames.timeSeq.n_rows << ","
       << frames.timeSeq.n_cols << endl;
}
