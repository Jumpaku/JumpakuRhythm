#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include "ExtractNotes.h"
#include "VideoAnalysis.h"
#include "BSplineCurve.hpp"

using namespace cv;
using namespace std;

/**
 * argv[1] : input file
 * argv[2] : output file
 */
int main(int argc, char** argv)
{
    if ( argc != 3 ) {
        cout << "usage : gen-data-init 'input file' 'output file'\n";
        return -1;
    }
    auto notes = drstVideo(argv[1], argv[2]);
    auto s = BSplineCurve::load(ifstream("data/NoteCurve.txt"));
    auto out = ofstream(string(argv[2]) + "/data.tsv");
    for_each(notes.begin(), notes.end(), [&out, s](auto n) {
        if (35 < n.y && n.y < 685) {
            out << n.timeMsec - s(n.y)<< " " << n.x << " " << n.y << "\n";
        }
    });
}
