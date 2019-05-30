#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct PreprocessArgs
{
    cv::Rect cropRect;
    cv::Size resizedSize;
};

struct FilterArgs
{
    cv::Scalar hsvLow;
    cv::Scalar hsvHigh;
};

cv::Mat correctPosture(cv::Mat const &frame);

cv::Mat preprocess(cv::Mat const &frame, PreprocessArgs const &args);

cv::Mat filterHsv(cv::Mat const &frame, FilterArgs const &args);

std::list<cv::Point2d> extractNotesInFrame(cv::Mat const &binaryFrame);


struct VideoInfo
{
    double width;
    double height;
    double fps;
    int frames;
};

VideoInfo getVideoInfo(cv::VideoCapture const &capture);

void forEachVideoFrames(
    cv::VideoCapture &&capture,
    std::function<void(int const index, double const msec, cv::Mat const &frame)> const &action
);

void transformVideoFrames(
    cv::VideoCapture &&capture,
    cv::VideoWriter &&writer,
    std::function<cv::Mat(int const index, double const msec, cv::Mat const &frame)> const &transform
);

void extractNoteObjectsVideoFrames(cv::VideoCapture &&capture, cv::VideoWriter && writer);


struct NoteCentroid
{
    double timeMsec;
    double x;
    double y;
};

struct NoteData {
    double timeMsec;
};

std::list<NoteCentroid> collectNoteCentroids(cv::VideoCapture &&capture);

std::vector<NoteData> fixNotesAtMsec(std::vector<NoteData> const &notes, double const lengthMsec);
