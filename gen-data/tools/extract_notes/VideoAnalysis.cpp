#include "VideoAnalysis.h"
#include "ExtractNotes.h"

#include <vector>
#include <list>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

struct VideoInfo
{
    double width;
    double height;
    double fps;
    int frames;
};

VideoInfo getVideoInfo(VideoCapture const &capture)
{
    return {
        capture.get(CAP_PROP_FRAME_WIDTH),
        capture.get(CAP_PROP_FRAME_HEIGHT),
        capture.get(CAP_PROP_FPS),
        static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT))
    };
}

void forVideoCaptureFrames(
    VideoCapture &&capture,
    function<void(int const index, double const msec, Mat const &frame)> const action
)
{
    while(capture.isOpened()) {
        int index = capture.get(CAP_PROP_POS_FRAMES);
        double msec = capture.get(CAP_PROP_POS_MSEC);
        Mat frame;
        capture >> frame;
        if (frame.empty()) break;
        
        action(index, msec, frame);
    }
    capture.release();
}

std::vector<NoteData> drstNotesVideo(std::string const &inputFile, std::string const &outputDir)
{
    auto capture = VideoCapture(inputFile);
    auto vcInfo = getVideoInfo(capture);

    auto crop = Rect(350, 140, vcInfo.height-700, vcInfo.width-140);
    auto resize = Size(crop.width/8, crop.height);
    auto writer = VideoWriter(outputDir + "/out.mov", VideoWriter::fourcc('m', 'p', '4', 'v'), vcInfo.fps, resize, false);

    auto l = list<NoteData>();
    forVideoCaptureFrames(
        move(capture),
        [&l, &writer, outputDir, vcInfo, crop, resize](auto index, auto msec, auto frame){
            auto preproccessed = preprocess(frame, { crop, resize });
            auto filtered = filterHsv(preproccessed, { Scalar(170, 255*0.35, 255*0.8), Scalar(180, 255*0.65, 255) });
            auto points = extractNotes(filtered);
            transform(points.begin(), points.end(), back_inserter(l), [msec](auto const &p){
                return NoteData { msec, p.x, p.y };
            });
            for_each(points.begin(), points.end(), [&filtered](auto const &p){
                filtered.template at<uchar>(p) = 0;
            });
            /*
            auto ss = stringstream();
            ss << outputDir << "/frame_" << right << setfill('0') << setw(to_string(vcInfo.frames).size() + 1) << index << ".jpg";
            imwrite(ss.str(), complessFrame(frame, rect, size));
            */
            writer.write(filtered);
    });

    writer.release();

    return vector<NoteData>(l.begin(), l.end());
}
