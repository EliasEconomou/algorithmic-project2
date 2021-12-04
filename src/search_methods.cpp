#include "../include/search_methods.hpp"

using namespace std;


// Function for i) assigment using LSH.
void time_series_LSH(string inputFile, string queryFile, string outputFile, int k, int L)
{
    cout << "time_series_LSH " << endl;
    if (inputFile == "0")
    {
        cout << "Give path to input file: ";
        cin >> inputFile;
    }

    Vector_of_points inputData;
    inputData = parsing(inputFile);

    // int dimension = inputData.points[0].vpoint.size();
    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < dimension; j++)
    //     {
    //         cout << inputData.points[i].vpoint[j] << "  ";
    //     }
    //     cout << endl << endl;
    // }
    
    int vectorsNumber = inputData.points.size();
    int dimension = inputData.points[0].vpoint.size();
    int bucketsNumber = vectorsNumber/8;
    LSH_hash_info hInfo(k, dimension, L);

    vector<HashTable> hashTables;
    for (int i = 0; i < L; i++)
    {
        HashTable ht(bucketsNumber);
        hashTables.push_back(ht);
    }
    
    for (int i = 0; i < L; i++)
    {
        hashTables[i].v = compute_v(k,dimension);
        hashTables[i].t = compute_t(k);
        hashTables[i].r = compute_r(k);
        for (int j = 0; j < vectorsNumber; j++)
        {
            hashTables[i].HTinsert(&inputData.points[j], &hInfo);
        }
    }

    // if (queryFile == "0")
    // {
    //     cout << "Give path to query file: ";
    //     cin >> queryFile;
    // }

    // if (outputFile == "0")
    // {
    //     cout << "Give path to output file: ";
    //     cin >> outputFile;
    // }

    // Vector_of_points queryData;
    // queryData = parsing(queryFile);
    
}

// Function for i) assigment using Hypercube.
void time_series_Hypercube(string inputFile, string queryFile, string outputFile, int k, int M, int probes)
{
    cout << "time_series_Hypercube" << endl;
}

// Function for ii) assigment using Discrete Ferchet.
void time_series_DiscreteFrechet(string inputFile, string queryFile, string outputFile, double delta)
{
    cout << "time_series_DiscreteFrechet" << endl;
}

// Function for iii) assigment using Continuous Frechet.
void time_series_ContinuousFrechet(string inputFile, string queryFile, string outputFile, double delta)
{
    cout << "time_series_ContinuousFrechet" << endl;
}