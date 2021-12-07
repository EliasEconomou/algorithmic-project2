#include "../include/grid_table.hpp"
#define M 100000 //used for padding grid-curves

using namespace std;


GridNode::GridNode(Curve *c, long int ID)
{
  this->ID = ID;
  this->curve = c;
}


GridTable::GridTable(int bucketsNumber, double delta, int dimension)
{
  this->bucketsNumber = bucketsNumber;
  this->tShiftGrid.first = random_double(0,delta);
  this->tShiftGrid.second = random_double(0,delta);
  this->delta = delta;
  this->dimension = dimension;
  this->lists.resize(this->bucketsNumber);
}


void GridTable::GridInsert(Curve *c, LSH_hash_info *hInfo)
{
  // We will create a grid-vector to snap curve to (as [value-1,time-1,...,value-m,time-m]) to be the hash key,
  // then use the LSH for vectors to find the right bucket.
  
  Curve grid_curve = snapToGrid(*c);
  padding(&grid_curve);
  vector<double> LSHvector = hashToLSHvector(grid_curve);
  // for (int i = 0; i < 2*this->dimension; i++)
  // {
  //   cout << LSHvector[i] << " - "; 
  // }
  // cout << endl << LSHvector.size() << endl;

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
  cout << "." << ID << ".  ";
  int g = compute_gValue(ID, this->bucketsNumber);
  lists[g].push_back(GridNode(c, ID));
  cout << "." << g << ".  ";
}


Curve GridTable::snapToGrid(Curve curve)
{
  cout << endl << endl << "CURVE ID : " << curve.curveID << endl << endl;
  Curve grid_curve;
  for (int i = 0; i < curve.coordinates.size(); i++)
  {

    double ay = floor((curve.coordinates[i].first - this->tShiftGrid.first)/this->delta + 0.5)*this->delta + this->tShiftGrid.first;
    double ax = floor((curve.coordinates[i].second - this->tShiftGrid.second)/this->delta + 0.5)*this->delta + this->tShiftGrid.second;
    // cout << "coming POINT " << i << " = " << curve.coordinates[i].first << " with TIME = " << curve.coordinates[i].second << " and GRID POINT = " << ay << " with TIME = " << ax << endl;
    pair<double,double> yxpoint = make_pair(ay,ax);
    int gridCsize = grid_curve.coordinates.size();

    grid_curve.coordinates.push_back(yxpoint);
  }
  // Remove consecutive duplicates.
  vector<pair<double,double> >::iterator it;
  it = unique(grid_curve.coordinates.begin(), grid_curve.coordinates.end());
  grid_curve.coordinates.resize( it - grid_curve.coordinates.begin() );
  return grid_curve;
}


void GridTable::padding(Curve *curve)
{
  int cSize = curve->coordinates.size(); //size before padding
  if (cSize < this->dimension)
  {
    cout << this->dimension << " - " << cSize << endl;
    for (int i = 0; i < this->dimension - cSize; i++)
    {
      pair<double,int> MM = make_pair(M,M);
      curve->coordinates.push_back(MM);
    }
  }
}


vector<double> GridTable::hashToLSHvector(Curve curve)
{
  vector<double> LSHvector;
  for (int i = 0; i < this->dimension; i++)
  {
    double y = curve.coordinates[i].first;
    double x = curve.coordinates[i].second;
    LSHvector.push_back(y);
    LSHvector.push_back(x);
  }
  return LSHvector;
}