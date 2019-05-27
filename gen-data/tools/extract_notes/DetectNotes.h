#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

struct NoteData {
    double timeMsec;
    double x;
    double y;
};

void forVideoCaptureFrames(
    cv::VideoCapture &&capture,
    std::function<void(double const msec, cv::Mat const &frame)> const &action
);

std::vector<NoteData> drstNotesVideo(std::string const &inputFile, std::string const &outputDir);
