#ifndef GRID_TABLE_H
#define GRID_TABLE_H

#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "curve_functions.hpp"
#include "hash_functions.hpp"

class GridNode
{
public:
    long int ID;
    Curve *curve;
    GridNode(Curve *p, long int ID);
};

class GridTable
{
    int bucketsNumber;
    std::vector<std::list<GridNode>> lists; //a list for every bucket
    Curve snapToGrid(Curve curve);
    void padding(Curve *curve);
    std::vector<double> hashToLSHvector(Curve curve);
    
public:
    int dimension; //dimension of curves
    std::pair<double,double> tShiftGrid; //shift grid's y/x by this number
    double delta;
    std::vector<double> t;
    std::vector<std::vector<double> > v; //k vectors to use to compute every h
    std::vector<int> r;

    GridTable(int bucketsNumber, double delta, int dimension);
    
    // Insert curve in grid
    void GridInsert(Curve *p, LSH_hash_info *hInfo);

    // Display grid (debug)
    void GridDisplay();

    int get_bucketsNumber();

    std::list<GridNode> get_bucketList(int g);
};

#endif