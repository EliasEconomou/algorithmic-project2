
#include "../include/acutest.h"
#include "../include/point_functions.hpp"
#include "../include/cluster_methods.hpp"
#include "../include/hash_table.hpp"
#include "../include/hash_functions.hpp"
#include "../include/algorithms.hpp"


void cluster_LSH_test(void);
void cluster_Classic_Curves_test(void);
void cluster_Classic_test(void);
void cluster_Hypercube_test(void);


void cluster_Classic_test(void){
    Cluster_of_points expected_cluster;
    Vector_of_points data;
    ClassPoint p1;
    std::vector<double> p1_vec;
    for (int i = 1; i < 11; i++)
    {
        p1_vec.push_back( 1 );
    }
    p1.vpoint = p1_vec;
    p1.itemID = "1";

    ClassPoint p2;
    std::vector<double> p2_vec;
    for (int i = 1; i < 11; i++)
    {
        p2_vec.push_back( 50 );
    }
    p2.vpoint = p2_vec;
    p2.itemID = "2";

    for (int i = 0; i < 5; i++)
    {
        data.points.push_back(p1);
    }
    for (int i = 0; i < 5; i++)
    {
        data.points.push_back(p2);
    }
    auto old_buffer = std::cout.rdbuf(nullptr);
    expected_cluster = cluster_Classic(data, expected_cluster, 2);
    std::cout.rdbuf(old_buffer);

    for (int i = 0; i < expected_cluster.centroids.size(); i++)
    {
        string id;
        string previd;
        for (int j = 0; j < expected_cluster.points[i].points.size() ; j++)
        {
            id = expected_cluster.points[i].points[j].itemID;

            if (j==0)previd=id;
            previd=id;

            TEST_CHECK( id == previd );

            previd = id;
        }
        
    }
    
}

void cluster_LSH_test(void){
    Cluster_of_points expected_cluster;
    Vector_of_points data;
    ClassPoint p1;
    std::vector<double> p1_vec;
    for (int i = 1; i < 11; i++)
    {
        p1_vec.push_back( 1 );
    }
    p1.vpoint = p1_vec;
    p1.itemID = 1;

    ClassPoint p2;
    std::vector<double> p2_vec;
    for (int i = 1; i < 11; i++)
    {
        p2_vec.push_back( 50 );
    }
    p2.vpoint = p2_vec;
    p2.itemID = 2;

    for (int i = 0; i < 5; i++)
    {
        data.points.push_back(p1);
    }
    for (int i = 0; i < 5; i++)
    {
        data.points.push_back(p2);
    }
    auto old_buffer = std::cout.rdbuf(nullptr);
    expected_cluster = cluster_LSH(data, expected_cluster, 2 , 2 , 5);
    std::cout.rdbuf(old_buffer);


    int counter = 0;
    for (int i = 0; i < expected_cluster.centroids.size(); i++)
    {
        string id;
        string previd;
        for (int j = 0; j < expected_cluster.points[i].points.size() ; j++)
        {
            id = expected_cluster.points[i].points[j].itemID;

            if (j==0)previd=id;

            if(previd == id)counter++;

            previd = id;
        }
        
    }
    TEST_CHECK( counter > 3 );
}

void cluster_Hypercube_test(void){
    Cluster_of_points expected_cluster;
    Vector_of_points data;
    ClassPoint p1;
    std::vector<double> p1_vec;
    for (int i = 1; i < 11; i++)
    {
        p1_vec.push_back( 1 );
    }
    p1.vpoint = p1_vec;
    p1.itemID = 1;

    ClassPoint p2;
    std::vector<double> p2_vec;
    for (int i = 1; i < 11; i++)
    {
        p2_vec.push_back( 30 );
    }
    p2.vpoint = p2_vec;
    p2.itemID = 2;

    for (int i = 0; i < 10; i++)
    {
        data.points.push_back(p1);
    }
    for (int i = 0; i < 10; i++)
    {
        data.points.push_back(p2);
    }
    auto old_buffer = std::cout.rdbuf(nullptr);
    expected_cluster = cluster_Hypercube(data, expected_cluster, 2 , 4 , 2 , 5);
    std::cout.rdbuf(old_buffer);
    int counter = 0;
    for (int i = 0; i < expected_cluster.centroids.size(); i++)
    {
        string id;
        string previd;
        for (int j = 0; j < expected_cluster.points[i].points.size() ; j++)
        {
            id = expected_cluster.points[i].points[j].itemID;

            if (j==0)previd=id;

            if(previd == id)counter++;

            previd = id;
        }
        
    }

    TEST_CHECK( counter > 6 );
}


TEST_LIST = {
    { "Cluster Classic" , cluster_Classic_test }, 
    { "Cluster LSH" , cluster_LSH_test },
    { "Cluster Hypercube" , cluster_Hypercube_test },                          
    { NULL, NULL }
};