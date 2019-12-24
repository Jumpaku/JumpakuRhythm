#pragma once
#include <vector>
#include <vector>
#include <istream>

struct BSplineCurve
{
    static BSplineCurve load(std::istream &&stream);
    
    int const degree;
    
    std::vector<double> const controlPoints;
    
    std::vector<double> const knots;

    double const begin;

    double const end;

    BSplineCurve(int degree, std::vector<double> const &controlPoints, std::vector<double> const&knots)
    : degree(degree),
      controlPoints(controlPoints),
      knots(knots),
      begin(knots[degree]),
      end(knots[knots.size() - degree - 1]){}

    double operator()(double t) const;

    double basisHelper(double a, double b, double c, double d) const;
};

//void drstCurve();
