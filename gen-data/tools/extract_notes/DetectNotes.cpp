#include "DetectNotes.h"

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace cv;


VideoInfo getVideoInfo(VideoCapture const &capture)
{
    return {
        capture.get(CAP_PROP_FRAME_WIDTH),
        capture.get(CAP_PROP_FRAME_HEIGHT),
        capture.get(CAP_PROP_FPS),
        static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT))
    };
}


cv::Mat correctPosture(cv::Mat const &frame)
{
    Mat result;
    flip(frame, result, 1);
    transpose(result, result);
    return result;
}

cv::Mat preprocess(cv::Mat const &frame, PreprocessArgs const &args)
{
    Mat cropped = frame(args.cropRect);
    Mat resized;
    resize(cropped, resized, args.resizedSize);
    return resized;
}

cv::Mat filterHsv(cv::Mat const &frame, FilterArgs const &args)
{
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    auto l = args.hsvLow;
    auto h = args.hsvHigh;
    Mat filtered;
    inRange(hsv, l, h, filtered);
    Mat denoised = filtered;
    Mat k = getStructuringElement(MORPH_CROSS, Size(5, 5));
    morphologyEx(denoised, denoised, MORPH_OPEN, k);
    morphologyEx(denoised, denoised, MORPH_DILATE, k);
    return denoised;
}

std::list<cv::Point2d> extractNotesInFrame(cv::Mat const &binaryFrame)
{
    vector<vector<Point>> contours;
    findContours(binaryFrame, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    list<Point2d> points;
    for(auto const &cnt : contours) {
        auto m = moments(cnt, true);
        auto cx = m.m10/m.m00;
        auto cy = m.m01/m.m00;
        points.emplace_back(cx, cy);
    }
    return points;
}


void forEachVideoFrames(
    cv::VideoCapture &&capture,
    std::function<void(int const index, double const msec, cv::Mat const &frame)> const &action
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

void transformVideoFrames(
    cv::VideoCapture &&capture,
    cv::VideoWriter &&writer,
    std::function<cv::Mat(int const index, double const msec, cv::Mat const &frame)> const &transform
) {
    forEachVideoFrames(move(capture), [&writer, transform](auto index, auto msec, auto frame) {
        writer << transform(index, msec, frame);
    });
    writer.release();
}

void extractNoteObjectsVideoFrames(cv::VideoCapture &&capture, cv::VideoWriter && writer)
{
    auto vcInfo = getVideoInfo(capture);
    auto crop = Rect(350, 140, vcInfo.height-700, vcInfo.width-140);
    auto preprocessArgs = PreprocessArgs {
        Rect(350, 140, vcInfo.height-700, vcInfo.width-140),
        Size(crop.width/8, crop.height)
    };
    auto filterArgs = FilterArgs {
        Scalar(170, 255*0.35, 255*0.8),
        Scalar(180, 255*0.65, 255)
    };
    transformVideoFrames(
        move(capture),
        move(writer),
        [preprocessArgs, filterArgs, vcInfo](auto index, auto msec, auto frame){
            auto corrected = correctPosture(frame);
            auto preproccessed = preprocess(corrected, preprocessArgs);
            auto filtered = filterHsv(preproccessed, filterArgs);
            return filtered;
    });
}

std::list<NoteCentroid> collectNoteCentroids(cv::VideoCapture &&capture)
{
    auto centroids = list<NoteCentroid>();
    forEachVideoFrames(move(capture), [&centroids](auto index, auto msec, auto frame){
        Mat binary;
        inRange(frame, Scalar(127,127,127), Scalar(255,255,255), binary);
        auto inFrame = extractNotesInFrame(binary);
        transform(inFrame.begin(), inFrame.end(), back_inserter(centroids), [msec](auto const &p){
            return NoteCentroid { msec, p.x, p.y };
        });
    });
    return centroids;

}

std::vector<NoteData> fixNotesAtMsec(std::vector<NoteData> const &notes, double const lengthMsec)
{
    auto nData = notes.size();
    auto nMsec = static_cast<int>(lengthMsec);
    auto density = std::vector<double>(nMsec+1, 0.0);
    auto band = 32.0;
    auto d = band/2;
    for (auto const &note : notes)
    {
        auto cur = note.timeMsec;
        auto begin = std::max(0, static_cast<int>(cur - d));
        auto end = std::min(nMsec, static_cast<int>(cur + d));
        for (int t = begin; t <= end; ++t)
        {
            auto u = (t - cur)/band;
            density[t] += ((abs(u) <= 1.0) ? (15.0/16.0)*(1 - u*u)*(1 - u*u) : 0.0) / nData / band;
        }
    }

    auto notesAt = list<NoteData>();
    for (size_t cur = 0; cur <= nMsec; ++cur)
    {
        auto prev = (cur == 0) ? 0.0 : density[cur-1];
        auto next = (cur == nMsec - 1) ? 0.0 : density[cur+1];
        auto curDens = density[cur];
        if (prev < curDens && curDens > next) {
            notesAt.push_back({ static_cast<double>(cur) });
        }
        cout << cur << " " << curDens << "\n";
    }

    return vector<NoteData>(notesAt.begin(), notesAt.end());
}
