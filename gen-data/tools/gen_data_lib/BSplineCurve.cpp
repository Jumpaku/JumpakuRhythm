#include"BSplineCurve.hpp"
#include<opencv2/opencv.hpp>

#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;


/*void drstCurve()
{
    auto s = BSplineCurve::load(cin);
    int N = 500;
    Mat out(400, N, CV_8UC1, Scalar::all(0));
    for (int i = 0; i <= N; i++) {
        auto y = std::max(s.begin, std::min(s.end, s.begin*(N - i - 1)/(N - 1.0) + s.end*(i/(N - 1.0))));
        auto t = std::max(0, std::min(399, 340 + static_cast<int>(s(y))));
        out.at<char>(Point(i, t)) = 255;
        cout << i << " " << t << "\n";
    }    
    
    imwrite(string("asset/") + "curve_plot.jpg", out);
}*/

BSplineCurve BSplineCurve::load(std::istream &&stream) {
    int d, n, m;
    auto cp = list<double>();
    auto us = list<double>();
    while(stream) {
        string s;
        stream >> s;
        if (s == "degree") stream >> d;
        else if (s == "cpSize") stream >> n;
        else if (s == "knotSize") stream >> m;
        else if (s == "cp") {
            double x;
            stream >> x;
            cp.push_back(x);
        }
        else if (s == "knot") {
            double u;
            int p;
            stream >> u >> p;
            for (size_t i = 0; i < p; i++) us.push_back(u);
        }
    }
    return BSplineCurve(d, std::vector<double>(cp.begin(), cp.end()), std::vector<double>(us.begin(), us.end()));
}
double BSplineCurve::operator()(double t) const {
    int ok = degree;
    int ng = knots.size() - degree;
    while (std::abs(ng - ok) > 1) {
        auto mid = (ok + ng)/2;
        if (knots[mid] <= t) ok = mid;
        else ng = mid;
    }
    auto l = ok;
    auto result = controlPoints;
    for (int k = 1; k <= degree; ++k) {
        for (int i = l; i >= l - degree + k; --i) {
            auto aki = basisHelper(t, knots[i], knots[i + degree + 1 - k], knots[i]);
            result[i] = (1 - aki)*result[i - 1] + aki * result[i];
        }
    }
    return result[l];
}

double BSplineCurve::basisHelper(double a, double b, double c, double d) const {
    return std::isfinite((a-b)/(c-d)) ? (a-b)/(c-d) : 0.0;
}