#include "../include/grid_table.hpp"

using namespace std;


GridNode::GridNode(Curve *c, long int ID)
{
  this->ID = ID;
  this->curve = c;
}


GridTable::GridTable(int bucketsNumber, double delta, int curveDim, int gridDim)
{
  if (gridDim == 1)
  {
    this->gridDim = gridDim;
    this->epsilon = EPSILON;
    this->tShiftGrid.first = -1.0;
    this->tShiftGrid.second = -1.0;
  }
  else if (gridDim == 2)
  {
    this->gridDim = gridDim;
    this->tShiftGrid.first = random_double(0,delta);
    this->tShiftGrid.second = random_double(0,delta);
    this->epsilon = -1.0;
  }

  this->bucketsNumber = bucketsNumber;
  this->delta = delta;
  this->curveDim = curveDim;
  this->lists.resize(this->bucketsNumber);
}


void GridTable::GridInsert(Curve *c, LSH_hash_info *hInfo)
{
  vector<double> LSHvector;
  if (this->gridDim == 1)
  {
    cout << "1" << endl;
  }
  else if (this->gridDim == 2)
  {
    // We will create a 2D grid-vector to snap curve to (as [value-1,time-1,...,value-m,time-m]) to be the hash key,
    // then use the LSH for vectors to find the right bucket.
    
    Curve grid_curve = snapToGrid(*c, this->tShiftGrid, this->delta);
    padding(&grid_curve, this->curveDim);
    LSHvector = hashToLSHvector(grid_curve, this->curveDim);
    // for (int i = 0; i < 2*this->curveDim; i++)
    // {
    //   cout << LSHvector[i] << " - "; 
    // }
    // cout << endl << LSHvector.size() << endl;
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
  //cout << "." << ID << ".  ";
  int g = compute_gValue(ID, this->bucketsNumber);
  lists[g].push_back(GridNode(c, ID));
  //cout << "." << g << ".  ";
}


int GridTable::get_bucketsNumber()
{
  return this->bucketsNumber;
}


list<GridNode> GridTable::get_bucketList(int g)
{
  return this->lists[g];
}