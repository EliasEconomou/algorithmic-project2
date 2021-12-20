
#include "../include/acutest.h"
#include "../include/point_functions.hpp"
#include "../include/cluster_methods.hpp"
#include "../include/hash_table.hpp"
#include "../include/hash_functions.hpp"
#include "../include/algorithms.hpp"


void test_inner_prod(void)
{
    std::vector<double> v1 = {3.0, 3.7, 12.1};
    std::vector<double> v2 = {6.2, 1.7, 0.65};
    double result = inner_prod(v1,v2);
    TEST_CHECK(result == 32.755);
}



void
test_tutorial(void)
{
    void* mem;

    mem = malloc(10);
    TEST_CHECK(mem != NULL);

    mem = realloc(mem, 20);
    TEST_CHECK(mem != NULL);

    free(mem);
}

void
test_fail(void)
{
    int a, b;

    /* This condition is designed to fail so you can see what the failed test
     * output looks like. */
    a = 1;
    b = 2;
    TEST_CHECK(a + b == 5);

    /* Here is TEST_CHECK_ in action. */
    TEST_CHECK_(a + b == 5, "%d + %d == 5", a, b);

    /* We may also show more information about the failure. */
    if(!TEST_CHECK(a + b == 5)) {
        TEST_MSG("a: %d", a);
        TEST_MSG("b: %d", b);
    }

    /* The macro TEST_MSG() only outputs something when the preceding
     * condition fails, so we can avoid the 'if' statement. */
    TEST_CHECK(a + b == 3);
    TEST_MSG("a: %d", a);
    TEST_MSG("b: %d", b);
}

static void
helper(void)
{
    /* Kill the current test with a condition which is never true. */
    TEST_ASSERT(1 == 2);

    /* This never happens because the test is aborted above. */
    TEST_CHECK(1 + 2 == 2 + 1);
}

void
test_abort(void)
{
    helper();

    /* This test never happens because the test is aborted inside the helper()
     * function. */
    TEST_CHECK(1 * 2 == 2 * 1);
}

void cluster_LSH_test(void);
void cluster_Classic_Curves_test(void);
void cluster_Classic_test(void);
void cluster_Hypercube_test(void);

TEST_LIST = {
    { "Inner Product",     test_inner_prod },
    { "Cluster Classic" , cluster_Classic_test }, 
    { "Cluster LSH" , cluster_LSH_test },
    { "Cluster Hypercube" , cluster_Hypercube_test },                          
    { NULL, NULL }
};


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
