#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include "FrameProcess.h"
#include "DetectNotes.h"
#include "BSplineCurve.hpp"
#include <opencv2/core/utils/logger.hpp>

using namespace cv;
using namespace std;




/**
 * argv[1] : input video file
 * argv[2] : output directory
 */
int main(int argc, char** argv)
{
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    if ( argc != 3 ) {
        cout << "usage : gen-data-init 'input video file' 'output directory'\n";
        return -1;
    }

    auto curve = BSplineCurve::load(ifstream("data/DrstNoteCurve.txt"));

    auto notes = detectNotesFromVideo(argv[1], argv[2]);

    auto out = ofstream(string(argv[2]) + "/note-data.tsv");
    for_each(notes.begin(), notes.end(), [&out, curve](auto n) {
        if (35 < n.y && n.y < 685) {
            out << n.timeMsec - curve(n.y)<< " " << n.x << " " << n.y << "\n";
        }
    });
}
