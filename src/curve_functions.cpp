#include "../include/curve_functions.hpp"

using namespace std;


// Parse dataset and return a vector of dataset's vectors.
Vector_of_curves curve_parsing(string fileName){
    
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

    //getting line by line
    while(std::getline(file, line)) {

        int t=1; // the second coordinate represents time and will be given as integer incremented by one for every point in curve-time series
        
        //getting data from each line and creating vectors to store them
        istringstream line_stringstream(line);
        string word;
        Curve c;

        line_stringstream >> word; // Read first word - index_ID
   
        c.curveID = word;
        while(line_stringstream >> word) {
            
            // p.vpoint.push_back(stod(word));
            c.coordinates.push_back(make_pair(stod(word),t++));
        }

        //Check if same size vectors are created
        if (sizelim==0){
            sizelim=c.coordinates.size();
        }
        if (c.coordinates.size()!=sizelim){
            cout << "Error with input file - inconsistent record size. \n";
            exit(-1);
        }

        //Add newly created point to data
        data.curves.push_back(c);
        
    }
    file.close();
    return data;


}