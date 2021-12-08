#include "../include/hash_functions.hpp"
#define M_PAD 100000 //used for padding grid-curves

using namespace std;


// Returns w
int compute_w(void)
{
    return 50;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns the t vector with values in [0,w).
vector<double> compute_t(int k)
{
    vector<double> t;
    random_device rd;
    mt19937 generator(rd());
    for (int i = 0; i < k; i++)
    {
        uniform_real_distribution<double> d(0.0, float(compute_w()));
        double coordinate = d(generator);
        t.push_back(coordinate);
    }
    return t;
}


// Clear vector t and add new random values to it.
void LSH_hash_info::update_t(vector<double> t)
{
    this->t.clear();
    this->t = t;
}

void CUBE_hash_info::update_t(vector<double> t)
{
    this->t.clear();
    this->t = t;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns the v vector with values distributed according to the Gaussian distribution.
vector<vector<double> > compute_v(int k, int d)
{
    vector<vector<double> > v;
    random_device rd;
    mt19937 generator(rd());
    for (int i = 0; i < k; i++)
    {
        vector<double> vi;
        for (int j = 0; j < d; j++)
        {
            normal_distribution<double> d{0,1};
            double coordinate = d(generator);
            vi.push_back(coordinate);
        }
        v.push_back(vi);
        
    }
    return v;
}


// Clear vectors v and add new random values to them.
void LSH_hash_info::update_v(vector<vector<double> > v)
{
    this->v.clear();
    this->v = v; 
}

void CUBE_hash_info::update_v(vector<vector<double> > v)
{
    this->v.clear();
    this->v = v; 
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns the r vector to use in g function
vector<int> compute_r(int k)
{
    vector<int> r;
    for (int i = 0; i < k; i++)
    {
        int rValue = random_number(1,INT32_MAX);
        r.push_back(rValue);
    }
    return r;
}


// Clear vector r and add new random values to it.
void LSH_hash_info::update_r(std::vector<int> r)
{
    this->r.clear();
    this->r = r;
    // for (int i = 0; i < k; i++)
    // {
    //     int rValue = random_number(1,INT32_MAX);
    //     this->r.push_back(rValue);
    // }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns M to use in g function
long long int compute_M()
{
    long long int M = pow(2,32) - 5;
    return M;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructors //

LSH_hash_info::LSH_hash_info(int k, int d, int L)
{
    this->k = k;
    this->d = d;
    this->L = L;
    this->w = compute_w();
    this->M = compute_M();
    
}

CUBE_hash_info::CUBE_hash_info(int k, int d, int M, int probes, int maxHD)
{
    this->maxHD = maxHD;
    this->k = k;
    this->d = d;
    this->M = M;
    this->probes = probes;
    this->w = compute_w();
    this->MapHtoF.resize(this->k);
}

// Getters //

vector<vector<double> > LSH_hash_info::get_v()
{
    return this->v;
}

vector<vector<double> > CUBE_hash_info::get_v()
{
    return this->v;
}

vector<double> LSH_hash_info::get_t()
{
    return this->t;
}

vector<double> CUBE_hash_info::get_t()
{
    return this->t;
}

vector<int> LSH_hash_info::get_r()
{
    return this->r;
}

int LSH_hash_info::get_w()
{
    return this->w;
}

int CUBE_hash_info::get_w()
{
    return this->w;
}

int LSH_hash_info::get_k()
{
    return this->k;
}

int CUBE_hash_info::get_k()
{
    return this->k;
}

int LSH_hash_info::get_d()
{
    return this->d;
}

int LSH_hash_info::get_L()
{
    return this->L;
}

long int LSH_hash_info::get_M()
{
    return this->M;
}

int CUBE_hash_info::get_M()
{
    return this->M;
}

int CUBE_hash_info::get_probes()
{
    return this->probes;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns an h-value.
int compute_hValue(int i, vector<double> p, LSH_hash_info *hInfo)
{
    int hValue;
    vector<vector<double> > v = hInfo->get_v();
    vector<double> vi = v[i];
    double pv = inner_prod(p,vi); //compute inner product p*v

    vector<double> t = hInfo->get_t();
    double ti = t[i];

    int w = hInfo->get_w();
    
    hValue = floor(pv - ti)/w;
    // cout << "h" << i << " = " << hValue << " ";
    return hValue;
}

int compute_hValue(int i, vector<double> p, CUBE_hash_info *hInfo)
{
    int hValue;
    vector<vector<double> > v = hInfo->get_v();
    vector<double> vi = v[i];
    double pv = inner_prod(p,vi); //compute inner product p*v

    vector<double> t = hInfo->get_t();
    double ti = t[i];

    int w = hInfo->get_w();
    
    hValue = floor(pv - ti)/w;
    
    return hValue;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Check if h-key is in map and has an f-value. If not assign f-value and add it to map. Return the f-value.
int CUBE_hash_info::update_map(int i, int hValue)
{
    auto it = this->MapHtoF[i].find(hValue);
    if (it == this->MapHtoF[i].end())
    {
        int fValue = random_number(0,1);
        MapHtoF[i].insert({hValue,fValue});
        return fValue;
    }
    return it->second;
    
}

// Returns the f value that corresponds to the h value given.
int compute_fValue(int i, int hValue, CUBE_hash_info *hInfo)
{
    int fValue = hInfo->update_map(i, hValue);
    return fValue;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns the ID - value.
long int compute_IDvalue(std::vector<int> hValues, LSH_hash_info *hInfo)
{
    int k = hInfo->get_k();
    long int M = hInfo->get_M();
    vector<int> r = hInfo->get_r();
    long int ID = 0;
    if(hValues.size()!=r.size()) {
        cout << "Error h-values vector must have same dimension (k) as r vector" << endl;
        return -1;
    }
    for (int i = 0; i < k; i++) {
        long int sum = r[i]*hValues[i];
        ID += modulo(sum,M);
        ID = modulo(ID,M);
    }
    //cout << " ----  ID = " << ID << " " ;
    return ID;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns the g hash function - value.
int compute_gValue(long int ID, int bucketNumber) //lsh
{
    int g = modulo(ID,bucketNumber);
//    cout << " ----  g = " << g << ". " << endl;
    return g;
}

// Converts binary to decimal.
int binary_to_decimal(vector<int> bin, int k)
{
    int dec = bin[0];

    for (int i = 1; i < k; i++) {
        dec = dec << (1);
        dec = dec + bin[i];
    }
    return dec;
}

int compute_gValue(vector<int> fValues, CUBE_hash_info *hInfo) //cube
{
    int g = binary_to_decimal(fValues, hInfo->get_k());
    return g;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Curve snapToGrid(Curve curve, pair<double,double> tShiftGrid, double delta)
{
  //cout << endl << endl << "CURVE ID : " << curve.curveID << endl << endl;
  Curve grid_curve;
  for (int i = 0; i < curve.coordinates.size(); i++)
  {

    double ay = floor((curve.coordinates[i].first - tShiftGrid.first)/delta + 0.5)*delta + tShiftGrid.first;
    double ax = floor((curve.coordinates[i].second - tShiftGrid.second)/delta + 0.5)*delta + tShiftGrid.second;
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

void padding(Curve *curve, int dimension)
{
  int cSize = curve->coordinates.size(); //size before padding
  if (cSize < dimension)
  {
    //cout << dimension << " pad " << cSize << endl;
    for (int i = 0; i < dimension - cSize; i++)
    {
      pair<double,int> MM = make_pair(M_PAD,M_PAD);
      curve->coordinates.push_back(MM);
    }
  }
}

vector<double> hashToLSHvector(Curve curve, int dimension)
{
  vector<double> LSHvector;
  for (int i = 0; i < dimension; i++)
  {
    double y = curve.coordinates[i].first;
    double x = curve.coordinates[i].second;
    LSHvector.push_back(y);
    LSHvector.push_back(x);
  }
  return LSHvector;
}