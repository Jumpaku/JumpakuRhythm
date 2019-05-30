#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "DetectNotes.h"
#include "BSplineCurve.hpp"

using namespace cv;
using namespace std;

/**
 * argv[1] : input video file
 * argv[2] : output directory
 */
int main(int argc, char** argv)
{
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    {/* extracted note objects -> out.mov */
        auto capture = VideoCapture(string(argv[1]));
        auto vcInfo = getVideoInfo(capture);
        auto writer = VideoWriter(
            string(argv[2]) + "/out.mp4",
            VideoWriter::fourcc('m', 'p', '4', 'v'),
            vcInfo.fps,
            Size((vcInfo.height-700)/8, vcInfo.width-140), false);
        extractNoteObjectsVideoFrames(move(capture), move(writer));
    //*/
    }
    {/* collect centroids of nodes from video *//*
        auto capture = VideoCapture(string(argv[2]) + "/out.mp4");
        auto noteCentroids = collectNoteCentroids(move(capture));

        auto end = remove_if(noteCentroids.begin(), noteCentroids.end(), [](auto const &e){
            return e.y < 35 || 685 < e.y;
        });
        auto curve = BSplineCurve::load(ifstream("data/DrstNoteCurve.txt"));
        auto candList = list<NoteData>();
        transform(noteCentroids.begin(), end, back_inserter(candList), [curve](auto const &e){
            return NoteData { e.timeMsec - curve(e.y) };
        });
        auto candidates = vector<NoteData>(candList.begin(), candList.end());
        
        auto fixed = fixNotesAtMsec(candidates, candidates.back().timeMsec);

        auto out = ofstream(string(argv[2]) + "/note-data-msec.tsv");
        std::for_each(fixed.begin(), fixed.end(), [&out](auto n) {
            out << n.timeMsec << "\n";
        });
    //*/
    }
}
