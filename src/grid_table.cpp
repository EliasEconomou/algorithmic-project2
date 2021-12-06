#include "../include/grid_table.hpp"

using namespace std;


GridNode::GridNode(Curve *c, long int ID)
{
  this->ID = ID;
  this->curve = c;
}


GridTable::GridTable(int bucketsNumber, double tShiftGrid, double delta, int dimension)
{
  this->bucketsNumber = bucketsNumber;
  this->tShiftGrid = tShiftGrid;
  this->delta = delta;
  this->dimension = dimension;
  this->lists.resize(this->bucketsNumber);
}


void GridTable::GridInsert(Curve *c, LSH_hash_info *hInfo)
{
  vector<double> LSHvector; //vector-point will have grid-coordinates as [value-1,time-1,...,value-m,time-m]

  // Snap curve to 2D grid.
  for (int i = 0; i < c->coordinates.size(); i++)
  {
    double ay = (c->coordinates[i].first - this->tShiftGrid) / this->delta;
    double ax = (c->coordinates[i].second - this->tShiftGrid) / this->delta;
    LSHvector.push_back(ay);
    LSHvector.push_back(ax);
  }

  vector<int> hValues;
  hInfo->update_v(this->v);
  hInfo->update_t(this->t);
  hInfo->update_r(this->r);
  int k = hInfo->get_k();  

  for (int i = 0; i < k; i++)
  {
    hValues.push_back(compute_hValue(i, LSHvector, hInfo));
  }

  long int ID = compute_IDvalue(hValues, hInfo);
  
  int g = compute_gValue(ID, this->bucketsNumber);
  lists[g].push_back(GridNode(c, ID));
  cout << "." << g << ".  ";
}

