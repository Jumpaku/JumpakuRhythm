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
    auto extracted = extractNotesInFrame(argv[1], argv[2]);
    auto end = remove_if(extracted.begin(), extracted.end(), [](auto const &e){
        return !(35 < e.y && e.y < 685);
    });
    auto candList = list<NoteData>();
    transform(extracted.begin(), end, back_inserter(candList), [curve](auto const &e){
        return NoteData { e.timeMsec - curve(e.y) };
    });
    auto candidates = vector<NoteData>(candList.begin(), candList.end());
    auto fixed = fixNotesAtMsec(candidates, candidates.back().timeMsec);

    auto out = ofstream(string(argv[2]) + "/note-data-msec.tsv");
    std::for_each(fixed.begin(), fixed.end(), [](auto n) {
        out << n.timeMsec << "\n";
    });
}
