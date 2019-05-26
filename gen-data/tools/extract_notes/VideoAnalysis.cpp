#include "VideoAnalysis.h"
#include"ExtractNotes.h"

#include <vector>
#include <list>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

std::vector<NoteData> drstNotesVideo(std::string const &inputFile, std::string const &outputDir)
{
    auto capture = VideoCapture(inputFile);
    auto fps = capture.get(CAP_PROP_FPS);
    auto width = capture.get(CAP_PROP_FRAME_WIDTH);
    auto height = capture.get(CAP_PROP_FRAME_HEIGHT);
    auto frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));

    auto rect = Rect(350, 140, height-700, width-140);
    auto size = Size(rect.width/8, rect.height);
    auto writer = VideoWriter(outputDir + "/out.mov", VideoWriter::fourcc('m', 'p', '4', 'v'), fps, size, false);

    auto l = list<NoteData>();
    while (capture.isOpened())
    {
        auto index = static_cast<int>(capture.get(CAP_PROP_POS_FRAMES));
        auto msec = capture.get(CAP_PROP_POS_MSEC);

        Mat frame;
        capture >> frame;
        if (frame.empty()) break;

        auto extracted = extractNotes(complessFrame(frame, rect, size));
        auto notesPos = detectNotes(extracted);
        for(auto const &p : notesPos) {
            extracted.at<uchar>(p) = 0;
            l.push_back({ msec, p.x, p.y });
        }
        auto ss = stringstream();
        ss << outputDir << "/frame_" << right << setfill('0') << setw(to_string(frames).size() + 1) << index << ".jpg";
        //imwrite(ss.str(), complessFrame(frame, rect, size));
        writer.write(extracted);
    }
    
    capture.release();
    writer.release();

    return vector<NoteData>(l.begin(), l.end());
}
