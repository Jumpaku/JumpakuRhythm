#pragma once

#include<opencv2/opencv.hpp>
#include<list>

cv::Mat complessFrame(cv::Mat const &frame, cv::Rect const &rect, cv::Size const &size);
cv::Mat extractNotes(cv::Mat const &frame);
std::list<cv::Point2d> detectNotes(cv::Mat const &frame);