#ifndef CURVE_FUNCTIONS_H
#define CURVE_FUNCTIONS_H

#include <algorithm>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <random>


class Curve
{
public:
    std::string curveID; //index id of curve
    std::vector<std::pair<double,double> > coordinates; //vector contains pairs of value/time - points in time series
};

class Vector_of_curves
{
public:
    std::vector<Curve> curves; //vector of points
};



// Parse dataset and return a vector of dataset's vectors.
Vector_of_curves curve_parsing(std::string);

#endif