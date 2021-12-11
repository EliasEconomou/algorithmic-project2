#include "../include/grid_table.hpp"

using namespace std;


GridNode::GridNode(ClassCurve *c, long int ID)
{
  this->ID = ID;
  this->curve = c;
}


GridTable::GridTable(int bucketsNumber, double delta, int curveDim, int pointDim)
{
  if (pointDim == 1)
  {
    this->epsilon = EPSILON;
    this->tShiftGrid.first = -1.0;
    this->tShiftGrid.second = -1.0;
  }
  else if (pointDim == 2)
  {
    this->tShiftGrid.first = random_double(0,delta);
    this->tShiftGrid.second = random_double(0,delta);
    this->epsilon = -1.0;
  }
  this->pointDim = pointDim;
  this->bucketsNumber = bucketsNumber;
  this->delta = delta;
  this->curveDim = curveDim;
  this->lists.resize(this->bucketsNumber);
}


void GridTable::GridInsert(ClassCurve *c, LSH_hash_info *hInfo)
{
  // We will create a vector by snapping incoming curve to a 1D or 2D grid. This 'LSHvector' will be the hash key for LSH
  // to find the right bucket.
  vector<double> LSHvector;

  if (this->pointDim == 1) //todo delete this->curveDim opou de xreiazetai
  {
    //ClassCurve filteredCurve = *c; //todo delete
    filtering(c, this->epsilon);
    ClassCurve gridCurve = snapToGrid(*c,this->delta);
    minima_maxima(&gridCurve);
    padding(&gridCurve, this->curveDim);
    LSHvector = keyLSHvector1D(gridCurve);
    // for (int i = 0; i < this->curveDim; i++)
    // {
    //   cout << LSHvector[i] << " - "; 
    // }
    // cout << endl << LSHvector.size() << endl;
  }
  else if (this->pointDim == 2)
  {
    ClassCurve gridCurve = snapToGrid(*c, this->tShiftGrid, this->delta);
    padding(&gridCurve, this->curveDim);
    LSHvector = keyLSHvector2D(gridCurve);
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