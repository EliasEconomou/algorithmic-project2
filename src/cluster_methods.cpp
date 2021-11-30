#include "../include/cluster_methods.hpp"
#define MAX_HD 6

using namespace std;



double silhuette(Cluster_of_points cluster, int i){
    double si=0;
    double sp;
    double sum_of_sps;
    for (int j = 0; j < cluster.points[i].points.size(); j++)
    {
        //FINDING SECOND CLOSEST CENTROID
        double sec_min_dist = MAXFLOAT;
        int sec_min_dist_it = -1;
        for (int k = 0; k < cluster.centroids.size(); k++)
        {
            if ( k == i )continue;
            double dist = distance(cluster.points[i].points[j].vpoint , cluster.centroids[k].vpoint , 2);
            if ( dist < sec_min_dist ){
                sec_min_dist = dist;
                sec_min_dist_it = k;
            }
        }
        double ai = 0;
        double dist = 0;
        for (int k = 0; k < cluster.points[i].points.size() ; k++)
        {
            if (k==j)continue;
            dist = distance(cluster.points[i].points[j].vpoint , cluster.points[i].points[k].vpoint , 2);
            ai += dist;
        }
        ai = ai / cluster.points[i].points.size();

        double bi = 0;
        for (int k = 0; k < cluster.points[sec_min_dist_it].points.size() ; k++)
        {
            double dist = distance(cluster.points[i].points[j].vpoint , cluster.points[sec_min_dist_it].points[k].vpoint , 2);
            bi += dist;
        }

        if( cluster.points[sec_min_dist_it].points.size() != 0 )bi = bi / cluster.points[sec_min_dist_it].points.size();
        else bi=0;

        double max;
        if (ai>bi)max=ai;
        else max=bi;

        if (max != 0 ){
            sp = (bi - ai) / max;
        }
        else sp=0;
        sum_of_sps += sp;
    }
    if ( cluster.points[i].points.size() != 0 )si = sum_of_sps / cluster.points[i].points.size();

    return si;
    
}

//---------------------------------------------------------------------------//
//                 CENTROID FUNCTION USED BY LLOYD'S ALGORYTHM               //
//---------------------------------------------------------------------------//

void calculate_centroids(Cluster_of_points &cluster){
    Point new_centroid;
    vector<Point> new_centroids;
    vector<double> sum_of_dimention;
    int centroid_count = cluster.centroids.size();
    
    // ---Set new centroids for cluster---

    //FOR EVERY CLUSTER OF POINTS
    for (int i = 0 ; i < cluster.centroids.size() ; i++ ){
        //FOR EVERY POINT IN CLUSTER
        if ( cluster.points[i].points.size() > 0 ){
            for (int j = 0 ; j < cluster.points[i].points.size() ; j++){
                //FOR EVERY DIMENTION IN VECTOR
                for (int k = 0 ; k < cluster.points[i].points[j].vpoint.size() ; k++){
                    if ( j == 0 ){
                        new_centroid.vpoint.push_back(cluster.points[i].points[j].vpoint[k]);
                    }
                    else{
                        new_centroid.vpoint[k]+=cluster.points[i].points[j].vpoint[k];
                    }
                }
            }
            for (int k = 0 ; k < new_centroid.vpoint.size() ; k++){
                new_centroid.vpoint[k] = new_centroid.vpoint[k] / cluster.points[i].points.size(); 
            }
            new_centroid.itemID=0;
            new_centroids.push_back(new_centroid);
            new_centroid.vpoint.clear();
        }
        else{
            for (int k = 0 ; k < cluster.centroids[i].vpoint.size() ; k++){
                new_centroid.vpoint.push_back(cluster.centroids[i].vpoint[k]);
            }
            new_centroid.itemID=cluster.centroids[i].itemID;
            new_centroids.push_back(new_centroid);
            new_centroid.vpoint.clear();
        }
        sum_of_dimention.clear();
    }
    cluster.centroids.swap(new_centroids);

   
}

//---------------------------------------------------------------------------//
//           K++ INITIALIZING FUNCTION ON CLUSTER OF POINTS                  //
//---------------------------------------------------------------------------//

Cluster_of_points initialize_kplusplus(Vector_of_points &Data, Cluster_of_points &cluster, int number_of_clusters){
    // ---Creating Kplusplus iteam with data structures to help---
    kplusplus_helper Kplusplus;

    // ---Getting a random point to be a centroid and adding it to centroid vector---
    Point Rand_centroid = Data.points[ random_number(1,Data.points.size()) ]; //Get random point to be the first centroid
    Kplusplus.Centroids.push_back(Rand_centroid);

    int t=1;


    // ---LOOP TO FIND NEW CENTROIDS---
    while (Kplusplus.Centroids.size() < number_of_clusters){
        //---Calculating all distances to centroids---
        bool wascentroid;
        vector<double> distances;
        for (int i=0 ; i < Data.points.size() ; i++){
            Point* CurrentPoint = &(Data.points[i]);
            wascentroid=false;
            for (int j=0 ; j < Kplusplus.Centroids.size() ; j++){
                if (Kplusplus.Centroids[j].itemID == CurrentPoint->itemID){ //if current is centroid dont
                    distances.push_back(0);
                    wascentroid=true;
                }
                else{
                    distances.push_back( distance( Kplusplus.Centroids[j].vpoint , CurrentPoint->vpoint , 2) );
                }
            }
            if(wascentroid){
                Kplusplus.IsCentroid.push_back(true);
            }
            else{
                Kplusplus.IsCentroid.push_back(false);
            }
            Kplusplus.Dist_From_Centroids.push_back(distances);
            distances.clear();
        }

        // ---Calculating minimum distances---
        for (int i=0 ; i < Data.points.size() ; i++){
            double min_dist = MAXFLOAT;
            for (int j=0 ; j < Kplusplus.Centroids.size() ; j++){
                if (Kplusplus.Dist_From_Centroids[i][j] < min_dist){
                    min_dist = Kplusplus.Dist_From_Centroids[i][j];
                }
            }
            // ---Saving minimum distances---
            Kplusplus.Minimum_Distances.push_back(min_dist);
        }

        // ---Calculating max D(i) to normalize---
        float max_di = 0;
        for (int i=0 ; i < Data.points.size() ; i++){
            if (Kplusplus.Minimum_Distances[i] > max_di){
                max_di = Kplusplus.Minimum_Distances[i];
            }
        }
        // ---Normalising and calculating cumulative sum of squares---
        for (int i=0 ; i < Data.points.size() ; i++){
            //Normalising
            float norm_distance = Kplusplus.Minimum_Distances[i] / max_di ;

            //Calculate sqare of normalised distance
            float norm_dist_squared = norm_distance * norm_distance;

            //Adding to vector of cumulative sums
            if (i==0){
                Kplusplus.Additive_Square_Sums.push_back(norm_dist_squared);
            }
            else{
                Kplusplus.Additive_Square_Sums.push_back(norm_dist_squared + Kplusplus.Additive_Square_Sums[i-1]);
            }
        }

        // ---Calculating the probabilities to be centroids---
        double uniform_rand_possibility=0;
        
        uniform_rand_possibility = random_double(0.0 , Kplusplus.Additive_Square_Sums[Kplusplus.Additive_Square_Sums.size()-1]);


        // ---Searching for next centroid according to random number taken---
        int next_centroid_index;
        for (int i=0 ; i < Data.points.size() ; i++){
            if ( (Kplusplus.Additive_Square_Sums[i] >= uniform_rand_possibility) && (Kplusplus.IsCentroid[i]==false) ){
                next_centroid_index = i;
                break;
            }
        }

        // ---Making it a centroid---
        
        Kplusplus.IsCentroid[next_centroid_index]=true;
        Kplusplus.Centroids.push_back(Data.points[next_centroid_index]);


        //Clearing for next loop
        Kplusplus.Minimum_Distances.clear();
        Kplusplus.Dist_From_Centroids.clear();
        Kplusplus.Additive_Square_Sums.clear();
        Kplusplus.IsCentroid.clear();
        t++;
    }   

    //Αssign centroids found to cluster and return
    for (int i=0 ; i < Kplusplus.Centroids.size() ; i++){
        cluster.centroids.push_back( Kplusplus.Centroids[i] );
    }
    return cluster;
}

//---------------------------------------------------------------------------//
//                 LLOYDS FUNCTION ON CLUSTER OF POINTS                      //
//---------------------------------------------------------------------------//

Cluster_of_points lloyds(Vector_of_points &Data, Cluster_of_points &cluster, int iter_num_input){

    int iter_num = iter_num_input;

    // ---Manually preallocating the vectors to load iteams without problems---
    Vector_of_points current_cluster;
    for (int i=0 ; i < cluster.centroids.size() ; i++){
        cluster.points.push_back(current_cluster);
    }


    // ---FOR ITER_NUM ITERATION OF THE ALGORYTHM---
    while (iter_num > 0){
        // ---ASSIGN EACH POINT TO A CENTROID---
        int min_centroid_iterator;
        double min_centroid_distance=MAXFLOAT;
        double dist;
        bool is_centroid;

        // ---ITERATING THROUGH POINTS---
        for (int i=0 ; i < Data.points.size() ; i++){

            //---Checking if point is centroid, if so ignoring it---
            is_centroid = false;
            for (int j=0 ; j < cluster.centroids.size() ; j++){
                if (cluster.centroids[j].itemID == Data.points[i].itemID){
                    is_centroid = true;
                    break;
                }

            }
            if (is_centroid)continue;

            // ---Creating clusters of points by assigning them to closest centroid---
            for (int j=0 ; j < cluster.centroids.size() ; j++){
                //Calculating distance between current point and current centroid
                dist = distance( Data.points[i].vpoint, cluster.centroids[j].vpoint , 2 );

                if (j==0){
                    min_centroid_iterator = 0;
                    min_centroid_distance = dist;
                }
                else{
                    if (dist < min_centroid_distance){
                        min_centroid_distance = dist;
                        min_centroid_iterator = j;
                    }
                }
            }
            cluster.points[min_centroid_iterator].points.push_back(Data.points[i]);

        }

        iter_num--;

        // ---IF NOT OVER , CLEANING UP FOR NEXT ITERATION---
        if (iter_num > 0){
            // ---ASSIGNING NEW CENTROIDS-- 
            calculate_centroids(cluster);

            //CLEANUP POINTS TO BE ABLE TO BE REASSIGNED
            for(int j=0 ; j < cluster.centroids.size() ; j++){
                cluster.points[j].points.clear();
            }
        }
    }

    return cluster;
}

//---------------------------------------------------------------------------//
//    CLUSTER 'CLASSIC' FUNCTION USING LLOYDS ON CLUSTER OF POINTS           //
//---------------------------------------------------------------------------//

Cluster_of_points cluster_Classic(Vector_of_points &Data, Cluster_of_points &cluster, int number_of_clusters){

    //INITIALIZE WITH K++
    cluster = initialize_kplusplus(Data, cluster, number_of_clusters);


    //CALCULATE LLOYDS ITERATIONS AS Log<database_size>
    int iter_lloyd=0;
    int datasize = Data.points.size();
    while (datasize/2 > 1){
        datasize = datasize/2;
        iter_lloyd++;
    }

    // ---LLOYDS ALGORYTHM---
    cluster = lloyds(Data, cluster, iter_lloyd);

    return cluster;
}

//---------------------------------------------------------------------------//
//              CLUSTER LSH FUNCTION ON CLUSTER OF POINTS                    //
//---------------------------------------------------------------------------//

Cluster_of_points cluster_LSH(Vector_of_points &Data, Cluster_of_points &cluster, int number_of_clusters, int L_of_LSH, int k_of_LSH){

    cluster = initialize_kplusplus(Data, cluster, number_of_clusters);

    // ---INITIALISE HASH TABLES FOR LSH---

    int vectorsNumber = Data.points.size();
    int dimension = Data.points[0].vpoint.size();
    int bucketsNumber = vectorsNumber/8;
    LSH_hash_info hInfo(k_of_LSH, dimension, L_of_LSH);

    vector<HashTable> hashTables;
    for (int i = 0; i < L_of_LSH; i++)
    {
        HashTable ht(bucketsNumber);
        hashTables.push_back(ht);
    }
    
    for (int i = 0; i < L_of_LSH; i++)
    {
        hashTables[i].v = compute_v(k_of_LSH,dimension);
        hashTables[i].t = compute_t(k_of_LSH);
        hashTables[i].r = compute_r(k_of_LSH);
        for (int j = 0; j < vectorsNumber; j++)
        {
            hashTables[i].HTinsert(&Data.points[j], &hInfo);
        }
    }


    bool stopflag=false;
    double R;
    unordered_map<int,int> Data_Found_map;
    unordered_map<int,int>::iterator it1;
    unordered_map<int,double> PointsInR;
    unordered_map<int,double>::iterator it2;

    // ---CALCULATING STARTING RANGE OF RANGE SEARCH AS HALF OF MINIMUM DISTANCE BETWEEN CENTROIDS--- 
    double min_dist = MAXFLOAT;
    for (int i=0 ; i < cluster.centroids.size() ; i++){
         for (int j=0 ; j < cluster.centroids.size() ; j++){
            if (i!=j){
                double dist = distance( cluster.centroids[i].vpoint , cluster.centroids[j].vpoint, 2 );
                if (dist < min_dist ){
                    min_dist = dist;
                }
            }
         }
    }
    R = min_dist / 2;

    // ---INITIALIZE CLUSTERS / PREALLOCATE STRUCTURES---
    for (int i = 0 ; i < Data.points.size() ; i++){
        Data_Found_map.insert(make_pair(Data.points[i].itemID, -1));
    }

    int turns_inactive = 0;
    bool first_action=false;
    while (!stopflag){
        bool tookaction=false;
        //FOR EVERY CENTROID
        for (int i=0 ; i < cluster.centroids.size() ; i++){

            PointsInR = lsh_approximate_range_search(cluster.centroids[i], R, hashTables, &hInfo);
            
            //FOR ALL POINTS FOUND BY RANGE SEARCH
            for (it2 = PointsInR.begin(); it2 != PointsInR.end(); it2++){
                int point_cluster_num = Data_Found_map.find(it2->first)->second;
                //IF POINT IS NOT YET FOUND, MAP IT TO CLUSTER
                if ( point_cluster_num == -1){
                    Data_Found_map.find(it2->first)->second = i;
                    tookaction=true;
                    first_action=true;
                    turns_inactive=0;
                    continue;
                }
                //IF POINT IS FOUND IN ANOTHER CLUSTER, COMPARE DISTANCES FROM CENTROIDS AND KEEP THE ONE WITH THE SMALLEST DISTANCE
                if ( point_cluster_num != i ){
                    int point_it = -1;
                    for (int j = 0 ; j < Data.points.size() ; j++){
                        if (Data.points[j].itemID == it2->first){
                            point_it = j;
                            break;
                        }
                    }
                    double min_dist = MAXFLOAT;
                    if ( distance( Data.points[point_it].vpoint, cluster.centroids[i].vpoint , 2 ) < distance(Data.points[point_it].vpoint, cluster.centroids[point_cluster_num].vpoint , 2 ) ){
                        it2->second = i;
                        tookaction=true;
                        turns_inactive=0;
                    }
                }
            }
            PointsInR.clear();
        }

        //CHECKING IF ANY ACTION WAS TAKEN THIS TURN
        if (!tookaction)turns_inactive++;
        //CHECKING IF NO POINTS HAVE BEEN ADDED IN 2 ITERATIONS, IF SO STOPPING
        if (turns_inactive > 1 && first_action)stopflag=true;

        //DOUBLING RANGE FOR EACH ITERATION
        R *=2;
        //IF R HAS REACHED MORE THAN 100K STOP
        if (R > 100000){
            stopflag=true;
        }
    }

    Vector_of_points newvec;
    for (int i = 0 ; i < cluster.centroids.size() ; i++){
        cluster.points.push_back(newvec);
    }

    // ---ARRANGE CLUSTER DATA ACCORDING TO MAP OF IDS TO CLUSTERS---
    for (it1 = Data_Found_map.begin(); it1 != Data_Found_map.end(); it1++){
        for (int i=0 ; i < Data.points.size() ; i++){
            if (it1->first == Data.points[i].itemID){
                //IF POINT NOT MAPPED TO ANY CLUSTER, FIND CLOSEST CENTROID AND ADD IT TO THAT CLUSTER
                if (it1->second == -1){
                    double min_dist = MAXFLOAT;
                    double dist;
                    int min_dist_it = -1;
                    for (int j=0 ; j < cluster.centroids.size() ; j++){
                        dist = distance( Data.points[i].vpoint , cluster.centroids[j].vpoint, 2 );
                        if ( dist < min_dist ){
                            min_dist = dist;
                            min_dist_it=j;
                        }
                    }
                    cluster.points[min_dist_it].points.push_back(Data.points[i]);
                }
                //OTHERWISE ADD IT TO MAPPED CLUSTER
                else{
                    cluster.points[it1->second].points.push_back(Data.points[i]);
                }
            }
        }
    }
    return cluster;
}

//---------------------------------------------------------------------------//
//              CLUSTER HYPERCUBE FUNCTION ON CLUSTER OF POINTS              //
//---------------------------------------------------------------------------//

Cluster_of_points cluster_Hypercube(Vector_of_points &Data, Cluster_of_points &cluster, int number_of_clusters, int M_of_Hypercube, int k_of_Hypercube, int probes_of_Hypercube){

    cluster = initialize_kplusplus(Data, cluster, number_of_clusters);

    // ---INITIALISE HASH TABLES FOR LSH---

    int vectorsNumber = Data.points.size();
    int dimension = Data.points[0].vpoint.size();
    int bucketsNumber = pow(2,k_of_Hypercube);
    CUBE_hash_info hInfo(k_of_Hypercube, dimension, M_of_Hypercube, probes_of_Hypercube, MAX_HD);

    CubeTable cubeTable(bucketsNumber);
    cubeTable.v = compute_v(k_of_Hypercube,dimension);
    cubeTable.t = compute_t(k_of_Hypercube);
    for (int i = 0; i < vectorsNumber; i++)
    {
        cubeTable.CTinsert(&Data.points[i], &hInfo);
    }



    bool stopflag=false;
    double R;
    unordered_map<int,int> Data_Found_map;
    unordered_map<int,int>::iterator it1;
    unordered_map<int,double> PointsInR;
    unordered_map<int,double>::iterator it2;

    //CALCULATING STARTING RANGE OF RANGE SEARCH AS HALF OF MINIMUM DISTANCE BETWEEN CENTROIDS
    double min_dist = MAXFLOAT;
    for (int i=0 ; i < cluster.centroids.size() ; i++){
         for (int j=0 ; j < cluster.centroids.size() ; j++){
             if (i!=j){
                double dist = distance( cluster.centroids[i].vpoint , cluster.centroids[j].vpoint, 2 );
                if (dist < min_dist ){
                    min_dist = dist;
                }
             }
         }
    }
    R = min_dist / 2;
    

    // ---INITIALIZE CLUSTERS / PREALLOCATE STRUCTURES---
    for (int i = 0 ; i < Data.points.size() ; i++){
        Data_Found_map.insert(make_pair(Data.points[i].itemID, -1));
    }

    int turns_inactive = 0;
    bool first_action=false;
    while (!stopflag){
        bool tookaction=false;
        //FOR EVERY CENTROID
        for (int i=0 ; i < cluster.centroids.size() ; i++){

            PointsInR = cube_approximate_range_search(cluster.centroids[i], R, cubeTable, &hInfo);
            
            //FOR ALL POINTS FOUND BY RANGE SEARCH
            for (it2 = PointsInR.begin(); it2 != PointsInR.end(); it2++){
                int point_cluster_num = Data_Found_map.find(it2->first)->second;
                //IF POINT IS NOT YET FOUND, MAP IT TO CLUSTER
                if ( point_cluster_num == -1){
                    Data_Found_map.find(it2->first)->second = i;
                    tookaction=true;
                    first_action=true;
                    turns_inactive=0;
                    continue;
                }
                //IF POINT IS FOUND IN ANOTHER CLUSTER, COMPARE DISTANCES FROM CENTROIDS AND KEEP THE ONE WITH THE SMALLEST DISTANCE
                if ( point_cluster_num != i ){
                    int point_it = -1;
                    for (int j = 0 ; j < Data.points.size() ; j++){
                        if (Data.points[j].itemID == it2->first){
                            point_it = j;
                            break;
                        }
                    }
                    double min_dist = MAXFLOAT;
                    if ( distance( Data.points[point_it].vpoint, cluster.centroids[i].vpoint , 2 ) < distance(Data.points[point_it].vpoint, cluster.centroids[point_cluster_num].vpoint , 2 ) ){
                        it2->second = i;
                        tookaction=true;
                        turns_inactive=0;
                    }
                }
            }
            PointsInR.clear();
        }

        //CHECKING IF ANY ACTION WAS TAKEN THIS TURN
        if (!tookaction)turns_inactive++;
        
        //CHECKING IF NO POINTS HAVE BEEN ADDED IN 2 ITERATIONS, IF SO STOPPING
        if (turns_inactive > 1 && first_action)stopflag=true;

        //DOUBLING RANGE FOR EACH ITERATION
        R *=2;

        //IF R HAS REACHED MORE THAN 100K STOP
        if (R > 100000){
            stopflag=true;
        }
    }

    Vector_of_points newvec;
    for (int i = 0 ; i < cluster.centroids.size() ; i++){
        cluster.points.push_back(newvec);
    }

    //---ARRANGE CLUSTER DATA ACCORDING TO MAP OF IDS TO CLUSTERS---
    for (it1 = Data_Found_map.begin(); it1 != Data_Found_map.end(); it1++){
        for (int i=0 ; i < Data.points.size() ; i++){
            if (it1->first == Data.points[i].itemID){
                //IF POINT NOT MAPPED TO ANY CLUSTER, FIND CLOSEST CENTROID AND ADD IT TO THAT CLUSTER
                if (it1->second == -1){
                    double min_dist = MAXFLOAT;
                    double dist;
                    int min_dist_it = -1;
                    for (int j=0 ; j < cluster.centroids.size() ; j++){
                        dist = distance( Data.points[i].vpoint , cluster.centroids[j].vpoint, 2 );
                        if ( dist < min_dist ){
                            min_dist = dist;
                            min_dist_it=j;
                        }
                    }
                    cluster.points[min_dist_it].points.push_back(Data.points[i]);
                }
                //OTHERWISE ADD IT TO MAPPED CLUSTER
                else{
                    cluster.points[it1->second].points.push_back(Data.points[i]);
                }
            }
        }
    }
    return cluster;
}
