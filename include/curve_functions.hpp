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
#include "point_functions.hpp" //include to get euclidean distance


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

// Compute discrete frechet distance between curves.
double discrete_frechet_distance (Curve c1, Curve c2);

#endif