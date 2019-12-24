#include "analysis.h"
#include "gnuplot-iostream.h"
#include "wavfile.h"
#include <iostream>

using namespace std;
using namespace arma;

int main(int argc, char **argv) {
  // read file's content
  /*
  WavAudio &&w = loadWav("/home/asset/2019-05-23_0753.wav");
  auto data = w.samples;
  auto hop_width = 1024;
  auto frame_width = 2048;
  auto frame_count = (w.data_count - frame_width) / hop_width;
  auto &&frames = makeFrames(w, hop_width, frame_width);

  auto t = makeTimeSeq(30, 10, 5);
  std::cout << t << std::endl;
  auto f = makeFreqSeq(30, 10, 5);
  std::cout << f << std::endl;
   */

  Gnuplot gp;
  int n = 10;
  std::vector<std::vector<double>> arr2d(n);
  for (size_t i = 0; i < n; ++i) {
    arr2d[i] = std::vector<double>(n);
    for (size_t j = 0; j < n; ++j) {
      arr2d[i][j] = i * (10.0 * j);
    }
  }

  // Don't forget to put "\n" at the end of each line!
  gp << "set terminal pdfcairo size 10cm, 10cm\n";
  gp << "set output \"plot.pdf\"\n";
  gp << "set dgrid3d 30,30\n";
  gp << "set hidden3d\n";
  gp << "plot \n";
  gp.send2d(arr2d);
}
