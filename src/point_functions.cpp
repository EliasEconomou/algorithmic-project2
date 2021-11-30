#include "../include/point_functions.hpp"

using namespace std;


// Function that returns vector of vectors, having data from the file at filepath "inputFile"
Vector_of_points parsing(string fileName){
    
    Vector_of_points data;

    //Opening input file
    ifstream file;
    file.open(fileName);
    if (file.is_open()==false){
        std::cout << "Error with given input file name - no file '" << fileName << "' was found or there was an error opening the file.\n";
        exit(-1);
    }
    string line;

    //sizelim : a size keeping variable to help with keeping consistent amount of dimentions
    int sizelim = 0;

    //getting line by line
    while(std::getline(file, line)) {

        //getting data from each line and creating vectors to store them
        istringstream line_stringstream(line);
        string word;
        Point p;

        line_stringstream >> word; // Read first word - index_ID
   
        p.itemID = stod(word);
        while(line_stringstream >> word) {
            
            p.vpoint.push_back(stod(word));
        }

        //Check if same size vectors are created
        if (sizelim==0){
            sizelim=p.vpoint.size();
        }
        if (p.vpoint.size()!=sizelim){
            cout << "Error with input file - inconsistent record size. \n";
            exit(-1);
        }

        //Add newly created point to data
        data.points.push_back(p);
        
    }
    file.close();
    return data;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Compute distance between vectors. L=1 for manhattan, L=2 for euclidian.
double distance(vector<int> v1, vector<int> v2, int L)
{
    int v1Dim = v1.size();
    int v2Dim = v2.size();
    if (v1Dim!=v2Dim)
    {
        cout << "Error, vectors have different size." << endl;
        return -1;
    }
    double sum = 0.0;
    for (int i = 0; i < v1Dim; i++)
    {
        sum += pow(abs(v1[i]-v2[i]), L);
    }
    return sqrt(sum);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Compute the inner product between two vectors.
double inner_prod(vector<int> v1, vector<double> v2)
{
    double innerProduct = 0.0;
    if(v1.size()!=v2.size()){
        cout << "Error in inner product, sizes not the same" << endl;
        return -1;
    }
    for (int i = 0; i < v1.size(); i++) {
        innerProduct = innerProduct + v1[i]*v2[i];
    }
    return innerProduct;
}
int inner_prod(vector<int> v1, vector<int> v2)
{
    int innerProduct = 0;
    if(v1.size()!=v2.size()){
        cout << "Error in inner product, sizes not the same" << endl;
        return -1;
    }
    for (int i = 0; i < v1.size(); i++) {
        innerProduct = innerProduct + v1[i]*v2[i];
    }
    return innerProduct;
}
double inner_prod(vector<double> v1, vector<double> v2)
{
    double innerProduct = 0.0;
    if(v1.size()!=v2.size()){
        cout << "Error in inner product, sizes not the same" << endl;
        return -1;
    }
    for (int i = 0; i < v1.size(); i++) {
        innerProduct = innerProduct + v1[i]*v2[i];
    }
    return innerProduct;
}
double inner_prod(vector<double> v1, vector<int> v2)
{
    double innerProduct = 0.0;
    if(v1.size()!=v2.size()){
        cout << "Error in inner product, sizes not the same" << endl;
        return -1;
    }
    for (int i = 0; i < v1.size(); i++) {
        innerProduct = innerProduct + v1[i]*v2[i];
    }
    return innerProduct;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns a random integer in the specified range.
int random_number(int begin, int end)
{
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> dis(begin, end);
    return dis(generator);
}

double random_double(double n1, double n2){
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(n1, n2);
    return dis(gen);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Returns modulo of two numbers.
long int modulo(long int a, long long int b)
{
    //cout << "a = " << a << " and b = " << b;
    return ((a%b)+b)%b;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////