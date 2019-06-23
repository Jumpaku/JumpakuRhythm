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

  cout << frames.frame_count << "," << frames.frames.n_rows << ","
       << frames.frames.n_cols << "," << frames.timeSeq.n_rows << ","
       << frames.timeSeq.n_cols << endl;
  */
  Gnuplot gp;
  std::vector<std::tuple<double, double, double, double>> pts_A;
  std::vector<double> pts_B_x;
  std::vector<double> pts_B_y;
  std::vector<double> pts_B_dx;
  std::vector<double> pts_B_dy;
  for (double alpha = 0; alpha < 1; alpha += 1.0 / 24.0) {
    double theta = alpha * 2.0 * 3.14159;
    pts_A.push_back(std::make_tuple(cos(theta), sin(theta), -cos(theta) * 0.1,
                                    -sin(theta) * 0.1));

    pts_B_x.push_back(cos(theta) * 0.8);
    pts_B_y.push_back(sin(theta) * 0.8);
    pts_B_dx.push_back(sin(theta) * 0.1);
    pts_B_dy.push_back(-cos(theta) * 0.1);
  }

  // Don't forget to put "\n" at the end of each line!
  gp << "set terminal pdfcairo size 10cm, 10cm\n";
  gp << "set output \"plot.pdf\"\n";
  gp << "set xrange [-2:2]\n";
  gp << "set yrange [-2:2]\n";
  gp << "plot '-' with vectors title 'pts_A', '-' with vectors title 'pts_B'\n";
  gp.send1d(pts_A);
  gp.send1d(std::make_tuple(pts_B_x, pts_B_y, pts_B_dx, pts_B_dy));
}
