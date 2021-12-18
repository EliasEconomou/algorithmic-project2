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
        ClassPoint p;

        line_stringstream >> word; // Read first word - index_ID
   
        p.itemID = word;
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
double distance(vector<double> v1, vector<double> v2, int L)
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


// Parse dataset and return a vector of dataset's vectors.
Vector_of_curves curve_parsing(string fileName, int dim){
    
    Vector_of_curves data;

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
    if (dim == 2) // time is an issue here
    {
        //getting line by line
        while(std::getline(file, line)) {

            double t=1; // the second coordinate represents time and will be given as integer incremented by one for every point in curve-time series
            int point_id=1;

            //getting data from each line and creating vectors to store them
            istringstream line_stringstream(line);
            string word;
            ClassCurve c;

            line_stringstream >> word; // Read first word - index_ID
    
            c.curveID = word;
            while(line_stringstream >> word) {
                
                //c.coordinates.push_back(make_pair(stod(word),t));
                ClassPoint p;
                p.itemID = to_string(point_id);
                p.vpoint.push_back(stod(word));
                p.vpoint.push_back(t);
                c.cpoints.push_back(p);
                point_id++;
                t=t+1;
            }

            //Check if same size curves are created
            if (sizelim==0){
                sizelim=c.cpoints.size();
            }
            if (c.cpoints.size()!=sizelim){
                cout << "Error with input file - inconsistent record size. \n";
                exit(-1);
            }

            //Add newly created point to data
            data.curves.push_back(c);
            
        }
    }
    else if (dim == 1)
    {
        //getting line by line
        while(std::getline(file, line)) {

            int point_id=1;

            //getting data from each line and creating vectors to store them
            istringstream line_stringstream(line);
            string word;
            ClassCurve c;

            line_stringstream >> word; // Read first word - index_ID
    
            c.curveID = word;
            while(line_stringstream >> word) {
                
                //c.coordinates.push_back(make_pair(stod(word),t));
                ClassPoint p;
                p.itemID = to_string(point_id);
                p.vpoint.push_back(stod(word));
                c.cpoints.push_back(p);
                point_id++;
            }

            //Check if same size curves are created
            if (sizelim==0){
                sizelim=c.cpoints.size();
            }
            if (c.cpoints.size()!=sizelim){
                cout << "Error with input file - inconsistent record size. \n";
                exit(-1);
            }

            //Add newly created point to data
            data.curves.push_back(c);
            
        }
    }
    
    
    
    file.close();
    return data;
}


double discrete_frechet_distance (ClassCurve c1, ClassCurve c2)
{
    int c1size = c1.cpoints.size();
    int c2size = c2.cpoints.size();
    double L[c1size][c2size];

    for (int i = 0; i < c1size; i++)
    {
        for (int j = 0; j < c2size; j++)
        {
            vector<double> c1point{c1.cpoints[i].vpoint[0],c1.cpoints[i].vpoint[1]};
            vector<double> c2point{c2.cpoints[j].vpoint[0],c2.cpoints[j].vpoint[1]};
            if ((i==0)&&(j==0))
            {
                L[i][j] = distance(c1point,c2point,2);
            }
            else if (i==0)
            {
                L[i][j] = max(distance(c1point,c2point,2),L[0][j-1]);
            }
            else if (j==0)
            {
                L[i][j] = max(distance(c1point,c2point,2),L[i-1][0]);
            }
            else
            {
                double prevmin = min(L[i-1][j],L[i][j-1]);
                prevmin = min(prevmin,L[i-1][j-1]);
                L[i][j] = max(distance(c1point,c2point,2),prevmin);
            }
        }
    }
    return L[c1size-1][c2size-1];



}


list<pair<int,int>> FindOptimalTraversal( ClassCurve c1, ClassCurve c2 ){
    // cout << "FINDING OPTIMAL TRAVERSAL" << endl;
    int c1size = c1.cpoints.size();
    int c2size = c2.cpoints.size();
    double L[c1size][c2size];

    for (int i = 0; i < c1size; i++)
    {
        for (int j = 0; j < c2size; j++)
        {
            vector<double> c1point{c1.cpoints[i].vpoint[0],c1.cpoints[i].vpoint[1]};
            vector<double> c2point{c2.cpoints[j].vpoint[0],c2.cpoints[j].vpoint[1]};
            if ((i==0)&&(j==0))
            {
                L[i][j] = distance(c1point,c2point,2);
            }
            else if (i==0)
            {
                L[i][j] = max(distance(c1point,c2point,2),L[0][j-1]);
            }
            else if (j==0)
            {
                L[i][j] = max(distance(c1point,c2point,2),L[i-1][0]);
            }
            else
            {
                double prevmin = min(L[i-1][j],L[i][j-1]);
                prevmin = min(prevmin,L[i-1][j-1]);
                L[i][j] = max(distance(c1point,c2point,2),prevmin);
            }
        }
    }

    std::list<std::pair<int,int>> OptimalTraversal;
    int Pi,Qi;
    Pi = c1size;
    Qi = c2size;
    OptimalTraversal.push_back( make_pair(Pi,Qi) );

    while (Pi != 0 && Qi != 0){
        int min_value_index;
        bool min_value_flag = min(L[Pi-1][Qi],L[Pi][Qi-1]) == L[Pi-1][Qi];
        if (min_value_flag){
            min_value_flag = min(L[Pi-1][Qi],L[Pi-1][Qi-1]) == L[Pi-1][Qi];
            if(min_value_flag)min_value_index=0;
            else min_value_index=2;
        }
        else {
            min_value_flag = min(L[Pi][Qi-1],L[Pi-1][Qi-1]) == L[Pi][Qi-1];
            if (min_value_flag)min_value_index=1;
            else min_value_index=2;
        }
        
        if (min_value_index == 0){
            OptimalTraversal.push_front( make_pair(--Pi,Qi) );
        }
        else if(min_value_index == 1){
            OptimalTraversal.push_front( make_pair(Pi,--Qi) );
        }
        else{
            OptimalTraversal.push_front( make_pair(--Pi,--Qi) );
        }
    }

    cout << "OPTIMAL TRAVERSAL | SIZE = " << OptimalTraversal.size() << endl;
    for (std::list<pair<int,int>>::iterator it=OptimalTraversal.begin() ; it != OptimalTraversal.end(); ++it)
    {
        cout << "(" << it->first << "," << it->second << ")";
    }
    cout << endl ;cout << endl ;cout << endl ;
    

    return OptimalTraversal;
}

ClassCurve Mean2Curves( ClassCurve c1, ClassCurve c2 ){
    // cout << "CALCULATING MEAN2CURVES" << endl;
    list<pair<int,int>> OptimalTraversal = FindOptimalTraversal(c1,c2);
    ClassCurve Mean;
    int traversalSize = OptimalTraversal.size();
    cout << OptimalTraversal.size() << endl;

    // list<pair<int,int>>::iterator it;
    for (int i = 0; i < traversalSize ; i++)
    {
        cout << "'" << OptimalTraversal.size() << "' ";
        pair<int,int> Current_pair;
        Current_pair = OptimalTraversal.front();
        cout << "(" << Current_pair.first << "," << Current_pair.second << ")";
 
        OptimalTraversal.pop_front();

        ClassPoint newPoint;
        // newPoint.vpoint.push_back(0);
        double value = (c1.cpoints[Current_pair.first].vpoint[0] + c2.cpoints[Current_pair.second].vpoint[0]) / 2;
        newPoint.vpoint.push_back(value);
        // newPoint.vpoint[0] = (c1.cpoints[Current_pair.first].vpoint[0] + c2.cpoints[Current_pair.second].vpoint[0]) / 2;
  
        value = (c1.cpoints[Current_pair.first].vpoint[1] + c2.cpoints[Current_pair.second].vpoint[1]) / 2;
        // newPoint.vpoint[1] = (c1.cpoints[Current_pair.first].vpoint[1] + c2.cpoints[Current_pair.second].vpoint[1]) / 2;
        newPoint.vpoint.push_back(value);
        newPoint.itemID = "0";

        cout << "NEW POINT CREATED";
        cout << "(" << newPoint.vpoint[0] << "," <<  newPoint.vpoint[1] << ") ";
  
        Mean.cpoints.push_back(newPoint);
        cout << Mean.cpoints.size();
    }
    cout << "LOOP ENDED" << endl;
    Mean.curveID = "0";
    return Mean;
}