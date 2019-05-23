
#include "ExtractNotes.h"

using namespace cv;
using namespace std;

cv::Mat complessFrame(cv::Mat const &frame, Rect const &rect)
{
    Mat rotated;
    flip(frame, rotated, 1);
    transpose(rotated, rotated);
    Mat cropped = rotated(rect);
    Mat resized;
    resize(cropped, resized, Size(rect.width/8, rect.height));
    return resized;
}

cv::Mat extractNotes(cv::Mat const &frame)
{
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    auto l = Scalar(170, 255*0.35, 255*0.8);
    auto h = Scalar(180, 255*0.65, 255);
    Mat filtered;
    inRange(hsv, l, h, filtered);
    Mat denoised = filtered;
    Mat k = getStructuringElement(MORPH_CROSS, Size(5, 5));
    morphologyEx(denoised, denoised, MORPH_OPEN, k);
    morphologyEx(denoised, denoised, MORPH_DILATE, k);
    return denoised;
}

std::list<cv::Point2d> detectNotes(cv::Mat const &frame)
{
    vector<vector<Point>> contours;
    findContours(frame, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    list<Point2d> points;
    for(auto const &cnt : contours) {
        auto m = moments(cnt, true);
        auto cx = m.m10/m.m00;
        auto cy = m.m01/m.m00;
        points.emplace_back(cx, cy);
    }
    return points;

}
