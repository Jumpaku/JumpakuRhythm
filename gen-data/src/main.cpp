#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "ExtractNotes.h"

using namespace cv;
using namespace std;

/**
 * argv[1] : input file
 * argv[2] : output file
 */
int main(int argc, char** argv )
{
    cout << argc << "\n";
    if ( argc != 3 )
    {
        cout << "usage : gen-data-init 'input file' 'output file'\n";
        return -1;
    }
    string input = argv[1];
    auto capture = VideoCapture(input);
    auto fps = capture.get(CAP_PROP_FPS);
    cout << "# fps : " << fps << "\n";
    auto w = capture.get(CAP_PROP_FRAME_WIDTH);
    cout << "# w : " << w << "\n";
    auto h = capture.get(CAP_PROP_FRAME_HEIGHT);
    cout << "# h : " << h << "\n";
    auto frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));
    cout << "# frames : " << frames << "\n";

    auto rect = Rect(350, 140, h-700, w-140);
    auto ss = stringstream();
    ss << argv[2] << "_" << right << setfill('0') << setw(to_string(frames).size() + 1) << ".mov";
    auto writer = VideoWriter(ss.str(), VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(rect.width/8, rect.height), false);
    
    while (capture.isOpened())
    {
        auto index = static_cast<int>(capture.get(CAP_PROP_POS_FRAMES));
        auto msec = capture.get(CAP_PROP_POS_MSEC);

        Mat frame;
        capture >> frame;
        if (frame.empty()) break;

        auto extracted = extractNotes(complessFrame(frame, rect));
        auto notesPos = detectNotes(extracted);
        for(auto const &p : notesPos) {
            extracted.at<uchar>((int)p.y, (int)p.x) = 0;
            cout << msec << "," << p.x << "," << p.y << "\n";
        }
        writer.write(extracted);
    }
    
    capture.release();
    writer.release();
    return 0;
}