#include "../include/search_methods.hpp"
#define MAX_HAMMING_DIST 10

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

    if (queryFile == "0")
    {
        cout << "Give path to query file: ";
        cin >> queryFile;
    }

    if (outputFile == "0")
    {
        cout << "Give path to output file: ";
        cin >> outputFile;
    }

    Vector_of_points queryData;
    queryData = parsing(queryFile);

    ofstream out (outputFile);

    std::cout << "Writing to output file..." << endl;
    for (int i = 0; i < queryData.points.size(); i++)
    {
        out << "Query: " << queryData.points[i].itemID << endl;
        double lshTime, trueTime;
        set<pair<Point,double>, CompDist> lshBestPointsDists;
        set<pair<Point,double>, CompDist> trueBestPointsDists;
        lshBestPointsDists = lsh_approximate_nNN(queryData.points[i], 1, hashTables, &hInfo, lshTime);
        trueBestPointsDists = true_nNN(queryData.points[i], 1, inputData, trueTime);
        int neighbor = 1;
        auto it1 = lshBestPointsDists.begin();
        auto it2 = trueBestPointsDists.begin();
        for (it1,it2; it1 != lshBestPointsDists.end(),it2 != trueBestPointsDists.end(); ++it1,++it2)
        {
            out << "Nearest neighbor-" << neighbor << ": " << it1->first.itemID << endl;
            out << "distanceLSH: " << it1->second << endl;
            out << "distanceTrue: " << it2->second << endl;
            neighbor++;
        }
        out << "tLSH: " << lshTime << endl;
        out << "tTrue: " << trueTime << endl;
    }
    out << endl;
    out.close();

    std::cout << "Operation completed successfully." << endl << "Exiting." << endl;
    
}

// Function for i) assigment using Hypercube.
void time_series_Hypercube(string inputFile, string queryFile, string outputFile, int k, int M, int probes)
{
    cout << "time_series_Hypercube" << endl;
    if (inputFile == "0")
    {
        cout << "Give path to input file: ";
        cin >> inputFile;
    }
    

    Vector_of_points inputData;
    inputData = parsing(inputFile);
    
    int vectorsNumber = inputData.points.size();
    int dimension = inputData.points[0].vpoint.size();
    int bucketsNumber = pow(2,k);
    CUBE_hash_info hInfo(k, dimension, M, probes, MAX_HAMMING_DIST);

    CubeTable cubeTable(bucketsNumber);
    cubeTable.v = compute_v(k,dimension);
    cubeTable.t = compute_t(k);
    for (int i = 0; i < vectorsNumber; i++)
    {
        cubeTable.CTinsert(&inputData.points[i], &hInfo);
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


    // ofstream out (outputFile);

    // std::cout << "Writing to output file..." << endl;
    // for (int i = 0; i < queryData.points.size(); i++)
    // {
    //     out << "Query: " << queryData.points[i].itemID << endl;
    //     double cubeTime, trueTime;
    //     set<pair<Point,double>, CompDist> cubeBestPointsDists;
    //     set<pair<Point,double>, CompDist> trueBestPointsDists;
    //     cubeBestPointsDists = cube_approximate_nNN(queryData.points[i], N, cubeTable, &hInfo, cubeTime);
    //     trueBestPointsDists = true_nNN(queryData.points[i], N, inputData, trueTime);
    //     int neighbor = 1;
    //     auto it1 = cubeBestPointsDists.begin();
    //     auto it2 = trueBestPointsDists.begin();
    //     for (it1,it2; it1 != cubeBestPointsDists.end(),it2 != trueBestPointsDists.end(); ++it1,++it2)
    //     {
    //         out << "Nearest neighbor-" << neighbor << ": " << it1->first.itemID << endl;
    //         out << "distanceHypercube: " << it1->second << endl;
    //         out << "distanceTrue: " << it2->second << endl;
    //         neighbor++;
    //     }
    //     out << "tHypercube: " << cubeTime << endl;
    //     out << "tTrue: " << trueTime << endl;
    //     unordered_map<int,double> PointsInR = cube_approximate_range_search(queryData.points[i], R, cubeTable, &hInfo);
    //     out << "R-near neighbors:" << endl;
    //     for (auto it = PointsInR.begin(); it != PointsInR.end(); ++it)
    //     {
    //         out << it->first << endl;
    //     }
    // }
    // out << endl;
    // out.close();
}

// Function for ii) assigment using Discrete Ferchet.
void time_series_DiscreteFrechet(string inputFile, string queryFile, string outputFile, int k, int L, double delta)
{
    cout << "time_series_DiscreteFrechet" << endl;
    if (inputFile == "0")
    {
        cout << "Give path to input file: ";
        cin >> inputFile;
    }

    std::cout << "Parsing data file..." << endl;
    Vector_of_curves inputData;
    inputData = curve_parsing(inputFile, 2);

    int curveDim = inputData.curves[0].cpoints.size(); // number of points in curves
    int pointDim = inputData.curves[0].cpoints[0].vpoint.size(); // dimension of points in curve
    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < dimension; j++)
    //     {
    //         cout << "(" << inputData.curves[i].cpoints[j].vpoint[0] << " - " << inputData.curves[i].cpoints[j].vpoint[1] << ")" << "  ";
    //     }
    //     cout << endl << endl;
    // }

    int curvesNumber = inputData.curves.size();
    int bucketsNumber = curvesNumber/8;
    
    LSH_hash_info hInfo(k, curveDim, L);

    vector<GridTable> gridTables;
    for (int i = 0; i < L; i++)
    {
        GridTable gt(bucketsNumber, delta, curveDim, pointDim);
        gridTables.push_back(gt);
    }
    
    std::cout << "Storing data in hash tables..." << endl;
    for (int i = 0; i < L; i++)
    {
        gridTables[i].v = compute_v(k,2*curveDim);
        gridTables[i].t = compute_t(k);
        gridTables[i].r = compute_r(k);
        for (int j = 0; j < curvesNumber; j++)
        {
            gridTables[i].GridInsert(&inputData.curves[j], &hInfo);
        }
    }

    if (queryFile == "0")
    {
        cout << "Give path to query file: ";
        cin >> queryFile;
    }

    if (outputFile == "0")
    {
        cout << "Give path to output file: ";
        cin >> outputFile;
    }

    Vector_of_curves queryData;
    queryData = curve_parsing(queryFile, pointDim);

    ofstream out (outputFile);

    std::cout << "Writing to output file..." << endl;
    for (int i = 0; i < queryData.curves.size(); i++)
    {
        out << "Query: " << queryData.curves[i].curveID << endl;
        double lshTime, trueTime;
        pair<Curve,double> lshBestPointDist;
        // pair<Point,double> trueBestPointsDists;
        lshBestPointDist = lsh_approximate_NN(queryData.curves[i], gridTables, &hInfo);
        out << "Nearest neighbor: " << lshBestPointDist.first.curveID << " and dist: " << lshBestPointDist.second << endl;
        // trueBestPointsDists = true_nNN(queryData.points[i], 1, inputData, trueTime);
        
    }
    out << endl;
    out.close();

    std::cout << "Operation completed successfully." << endl << "Exiting." << endl;

}

// Function for iii) assigment using Continuous Frechet.
void time_series_ContinuousFrechet(string inputFile, string queryFile, string outputFile, int k, int L, double delta)
{
    cout << "time_series_ContinuousFrechet" << endl;
    if (inputFile == "0")
    {
        cout << "Give path to input file: ";
        cin >> inputFile;
    }

    std::cout << "Parsing data file..." << endl;
    Vector_of_curves inputData;
    inputData = curve_parsing(inputFile, 1);

    int curveDim = inputData.curves[0].cpoints.size(); // number of points in curves
    int pointDim = inputData.curves[0].cpoints[0].vpoint.size(); // dimension of points in curve
    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < dimension; j++)
    //     {
    //         cout << "(" << inputData.curves[i].cpoints[j].vpoint[0] << ")" << "  ";
    //     }
    //     cout << endl << endl;
    // }

    int curvesNumber = inputData.curves.size();
    int bucketsNumber = curvesNumber/8;
    
    LSH_hash_info hInfo(k, curveDim, L);

    vector<GridTable> gridTables;
    for (int i = 0; i < L; i++)
    {
        GridTable gt(bucketsNumber, delta, curveDim, pointDim);
        gridTables.push_back(gt);
    }
    
    std::cout << "Storing data in hash tables..." << endl;
    for (int i = 0; i < L; i++)
    {
        gridTables[i].v = compute_v(k,curveDim);
        gridTables[i].t = compute_t(k);
        gridTables[i].r = compute_r(k);
        for (int j = 0; j < curvesNumber; j++)
        {
            gridTables[i].GridInsert(&inputData.curves[j], &hInfo);
        }
    }

    if (queryFile == "0")
    {
        cout << "Give path to query file: ";
        cin >> queryFile;
    }

    if (outputFile == "0")
    {
        cout << "Give path to output file: ";
        cin >> outputFile;
    }

    Vector_of_curves queryData;
    queryData = curve_parsing(queryFile, pointDim);

    // ofstream out (outputFile);

    // std::cout << "Writing to output file..." << endl;
    // for (int i = 0; i < queryData.curves.size(); i++)
    // {
    //     out << "Query: " << queryData.curves[i].curveID << endl;
    //     double lshTime, trueTime;
    //     pair<Curve,double> lshBestPointDist;
    //     // pair<Point,double> trueBestPointsDists;
    //     lshBestPointDist = lsh_approximate_NN(queryData.curves[i], gridTables, &hInfo);
    //     out << "Nearest neighbor: " << lshBestPointDist.first.curveID << " and dist: " << lshBestPointDist.second << endl;
    //     // trueBestPointsDists = true_nNN(queryData.points[i], 1, inputData, trueTime);
        
    // }
    // out << endl;
    // out.close();

    // std::cout << "Operation completed successfully." << endl << "Exiting." << endl;

}