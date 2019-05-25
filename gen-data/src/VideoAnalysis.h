#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

struct NoteData {
    double timeMsec;
    double x;
    double y;
};

std::vector<NoteData> drstVideo(std::string const &inputFile, std::string const &outputDir);

void drstCurve(int degree, std::vector<double> const &cp, std::vector<double> &us);