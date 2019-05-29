#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct NoteCandidate
{
    double timeMsec;
    double x;
    double y;
};

struct NoteData {
    double timeMsec;
};

void forVideoCaptureFrames(
    cv::VideoCapture &&capture,
    std::function<void(double const msec, cv::Mat const &frame)> const &action
);

std::vector<NoteCandidate> extractNotesInFrame(std::string const &inputFile, std::string const &outputDir);

std::vector<NoteData> fixNotesAtMsec(std::vector<NoteData> const &notes, double const lengthMsec);
