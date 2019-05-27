#pragma once

#include<opencv2/opencv.hpp>
#include<list>

struct PreproccessArgs
{
    cv::Rect cropRect;
    cv::Size resizedSize;
};

struct FilterArgs
{
    cv::Scalar hsvLow;
    cv::Scalar hsvHigh;
};

cv::Mat preproccess(cv::Mat const &frame, PreproccessArgs const &args);
cv::Mat filterHsv(cv::Mat const &frame, FilterArgs const &args);
std::list<cv::Point2d> extractNotes(cv::Mat const &frame);