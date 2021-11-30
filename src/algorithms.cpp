#include "../include/algorithms.hpp"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TRUE //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pair<Point,double> true_NN(Point q, Vector_of_points inputData)
{
    Point b;
    pair<Point,double> best;
    best.first = b; //best point-candidate
    best.second = DBL_MAX; //best distance of best candidate
    
    for (int i = 0; i < inputData.points.size(); i++)
    {
        double dist = distance(q.vpoint,inputData.points[i].vpoint, 2);
        if (dist < best.second)
        {
            best.second = dist;
            best.first = inputData.points[i];
        }
        
    }
    return best;

}


set<pair<Point,double>, CompDist> true_nNN(Point q, int N, Vector_of_points inputData, double &time)
{
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // Initialise a set two hold pairs of true best point/best distance.
    set<pair<Point,double>, CompDist> bestPointsDists;
    Point a;
    bestPointsDists.insert(make_pair(a,DBL_MAX));
    
    for (int i = 0; i < inputData.points.size(); i++)
    {
        double dist = distance(q.vpoint,inputData.points[i].vpoint, 2);
        if (bestPointsDists.size()==N) //if set is full
        {
            //if the biggest distance in set is equal/greater than current distance
            if (prev(bestPointsDists.end())->second >= dist)
            {
                bestPointsDists.erase(prev(bestPointsDists.end())); //pop biggest distance pair
                bestPointsDists.insert(make_pair(inputData.points[i],dist)); //insert new point/distance
            }
        }
        else if (bestPointsDists.size()<N) //if there is space in set insert pair
        {
            bestPointsDists.insert(make_pair(inputData.points[i],dist));
        }
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end - start);
    time = time_span.count();
    return bestPointsDists;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LSH //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


pair<Point,double> lsh_approximate_NN(Point q, vector<HashTable> hashTables, LSH_hash_info *hInfo)
{
    Point b;
    pair<Point,double> best;
    best.first = b; //best point-candidate
    best.second = DBL_MAX; //best distance of best candidate

    int L = hInfo->get_L();
    for (int i = 0; i < L; i++) {
        // Update hinfo with the right vectors for every hash table, to compute query's g-value
        hInfo->update_v(hashTables[i].v);
        hInfo->update_t(hashTables[i].t);
        hInfo->update_r(hashTables[i].r);
        // Find g value for query point.
        vector<int> hValues;
        int k = hInfo->get_k();
        vector<int> vp = q.vpoint;
        for (int j = 0; j < k; j++)
        {
            hValues.push_back(compute_hValue(j, vp, hInfo));
            
        }
        long int ID = compute_IDvalue(hValues, hInfo);
        int g = compute_gValue(ID, hashTables[i].get_bucketsNumber());
        list<HashNode> listToSearch = hashTables[i].get_bucketList(g);
        typename list<HashNode>::iterator current;
        for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
            if (ID != current->ID)
            {
                continue;
            }

            double dist = distance(q.vpoint,current->point->vpoint, 2);
            if (dist < best.second)
            {
                best.second = dist;
                best.first = *(current->point);
            }
        }
    }
    return best;
    
}


set<pair<Point,double>, CompDist> lsh_approximate_nNN(Point q, int N, vector<HashTable> hashTables, LSH_hash_info *hInfo, double &time)
{
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // Initialise a set two hold pairs of best point/best distance.
    set<pair<Point,double>, CompDist> bestPointsDists;
    Point a;
    bestPointsDists.insert(make_pair(a,DBL_MAX));
    int L = hInfo->get_L();
    for (int i = 0; i < L; i++) {
        // Update hinfo with the right vectors for every hash table, to compute query's g-value
        hInfo->update_v(hashTables[i].v);
        hInfo->update_t(hashTables[i].t);
        hInfo->update_r(hashTables[i].r);
        // Find g value for query point.
        vector<int> hValues;
        int k = hInfo->get_k();
        vector<int> vp = q.vpoint;
        for (int j = 0; j < k; j++)
        {
            hValues.push_back(compute_hValue(j, vp, hInfo));
            
        }
        long int ID = compute_IDvalue(hValues, hInfo);
        int g = compute_gValue(ID, hashTables[i].get_bucketsNumber());
        list<HashNode> listToSearch = hashTables[i].get_bucketList(g);
        typename list<HashNode>::iterator current;
        for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
            if (ID != current->ID)
            {
                continue;
            }
            
            double dist = distance(q.vpoint,current->point->vpoint, 2);
            if (bestPointsDists.size()==N) //if set is full
            {
                //if the biggest distance in set is equal/greater than current distance
                if (prev(bestPointsDists.end())->second >= dist)
                {
                    bestPointsDists.erase(prev(bestPointsDists.end())); //pop biggest distance pair
                    bestPointsDists.insert(make_pair(*(current->point),dist)); //insert new point/distance
                }
            }
            else if (bestPointsDists.size()<N) //if there is space in set insert pair
            {
                bestPointsDists.insert(make_pair(*(current->point),dist));
            }
        }
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end - start);
    time = time_span.count();
    return bestPointsDists;
}


unordered_map<int,double> lsh_approximate_range_search(Point q, double R, vector<HashTable> hashTables, LSH_hash_info *hInfo)
{
    // Initialise an unordered map two hold points-distances inside radius r.
    unordered_map<int,double> rPoints;
    int L = hInfo->get_L();
    for (int i = 0; i < L; i++) {
        // Update hinfo with the right vectors for every hash table, to compute query's g-value
        hInfo->update_v(hashTables[i].v);
        hInfo->update_t(hashTables[i].t);
        hInfo->update_r(hashTables[i].r);
        // Find g value for query point.
        vector<int> hValues;
        int k = hInfo->get_k();
        vector<int> vp = q.vpoint;
        for (int j = 0; j < k; j++)
        {
            hValues.push_back(compute_hValue(j, vp, hInfo));
            
        }
        long int ID = compute_IDvalue(hValues, hInfo);
        int g = compute_gValue(ID, hashTables[i].get_bucketsNumber());
        list<HashNode> listToSearch = hashTables[i].get_bucketList(g);
        typename list<HashNode>::iterator current;
        for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
            if (ID != current->ID)
            {  
                continue;
            }

            double dist = distance(q.vpoint,current->point->vpoint, 2);
            if (dist < R && dist != 0)
            {
                rPoints.insert(make_pair(current->point->itemID,dist));
            }
        }
    }
    return rPoints;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUBE //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int hammingDistance(int n1, int n2)
{
    int x = n1 ^ n2;
    int setBits = 0;
 
    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }
 
    return setBits;
}


pair<Point,double> cube_approximate_NN(Point q, CubeTable cubeTable, CUBE_hash_info *hInfo)
{
    Point b;
    pair<Point,double> best;
    best.first = b; //best point-candidate
    best.second = DBL_MAX; //best distance of best candidate
    
    // Update hinfo with the right vectors for hash table, to compute query's g-value
    hInfo->update_v(cubeTable.v);
    hInfo->update_t(cubeTable.t);
    // Find g value for query point.
    vector<int> hValues;
    int k = hInfo->get_k();
    vector<int> vp = q.vpoint;
    for (int i = 0; i < k; i++)
    {
        hValues.push_back(compute_hValue(i, vp, hInfo));
    }
    vector<int> fValues;
    for (int i = 0; i < k; i++)
    {
        fValues.push_back(compute_fValue(i, hValues[i], hInfo));
    }
    int g = compute_gValue(fValues, hInfo);

    int M = hInfo->get_M(); //maximum number of points to search
    int currentM = 0; //keep track how many points we have searched till now

    // We finished checking vertice g but we have more points to check (because currentM < M).
    // We can't check more than 'probes minus the g' vertices and we will search until hamming distance = maxHD.
    int maxProbes = hInfo->get_probes();
    int maxHD = hInfo->maxHD;
    int numVertices = cubeTable.get_bucketsNumber();
    int currentProbes = maxProbes; //current probes will be reduced every time a new vertex is checked
    
    // Loop hamming distances till maximum hd defined in cube program.
    for (int hd = 0; hd <= maxHD; hd++) 
    {
        vector<int> HDhasProbes; //will consist of all vertices-indexes of current hamming distance
        
        // Loop all vertices and store only the ones with hamming distance = hd from g-index.
        for (int i = 0; i < numVertices; i++)
        {
            if(hammingDistance(g,i) == hd)
            {
                HDhasProbes.push_back(i);
            }
        }
        random_shuffle(HDhasProbes.begin(), HDhasProbes.end()); //randomize vertices
        
        // Loop from 0 to maxProbes and access current hd's probes.
        for (int i = 0; i < maxProbes; i++)
        {
            if (i==HDhasProbes.size()) //current hd has no more probes - break and increment hd
            {
                break;
            }
                        
            // Finally search vertice for best distance.
            list<Vertice> listToSearch = cubeTable.get_bucketList(HDhasProbes[i]);
            typename list<Vertice>::iterator current;
            for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
                double dist = distance(q.vpoint,current->point->vpoint, 2);
                currentM++;
                if (dist < best.second)
                {
                    best.second = dist;
                    best.first = *(current->point);
                }
                if (currentM == M) //maximum points to check reached
                {
                    return best;
                }   
            }

            currentProbes--;
            // If no more probes to check return.
            if (currentProbes == 0)
            {
                return best;
            }
        }
        HDhasProbes.clear(); //clear table to get next hd's probes

    }
    // Max HD reached.
    return best;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


set<pair<Point,double>, CompDist> cube_approximate_nNN(Point q, int N, CubeTable cubeTable, CUBE_hash_info *hInfo, double &time)
{
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // Initialise a set two hold pairs of best point/best distance.
    set<pair<Point,double>, CompDist> bestPointsDists;
    Point a;
    bestPointsDists.insert(make_pair(a,DBL_MAX));
    
    // Update hinfo with the right vectors for hash table, to compute query's g-value
    hInfo->update_v(cubeTable.v);
    hInfo->update_t(cubeTable.t);
    // Find g value for query point.
    vector<int> hValues;
    int k = hInfo->get_k();
    vector<int> vp = q.vpoint;
    for (int i = 0; i < k; i++)
    {
        hValues.push_back(compute_hValue(i, vp, hInfo));
    }
    vector<int> fValues;
    for (int i = 0; i < k; i++)
    {
        fValues.push_back(compute_fValue(i, hValues[i], hInfo));
    }
    int g = compute_gValue(fValues, hInfo);

    int M = hInfo->get_M(); //maximum number of points to search
    int currentM = 0; //keep track how many points we have searched till now

    // We finished checking vertice g but we have more points to check (because currentM < M).
    // We can't check more than 'probes minus the g' vertices and we will search until hamming distance = maxHD.
    int maxProbes = hInfo->get_probes();
    int maxHD = hInfo->maxHD;
    int numVertices = cubeTable.get_bucketsNumber();
    int currentProbes = maxProbes; //current probes will be reduced every time a new vertex is checked
    
    // Loop hamming distances till maximum hd defined in cube program.
    for (int hd = 0; hd <= maxHD; hd++) 
    {
        vector<int> HDhasProbes; //will consist of all vertices-indexes of current hamming distance
        
        // Loop all vertices and store only the ones with hamming distance = hd from g-index.
        for (int i = 0; i < numVertices; i++)
        {
            if(hammingDistance(g,i) == hd)
            {
                HDhasProbes.push_back(i);
            }
        }
        random_shuffle(HDhasProbes.begin(), HDhasProbes.end()); //randomize vertices
        
        // Loop from 0 to maxProbes and access current hd's probes.
        for (int i = 0; i < maxProbes; i++)
        {
            if (i==HDhasProbes.size()) //current hd has no more probes - break and increment hd
            {
                break;
            }

            // Finally search vertice for best distance.
            list<Vertice> listToSearch = cubeTable.get_bucketList(HDhasProbes[i]);
            typename list<Vertice>::iterator current;
            for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
                double dist = distance(q.vpoint,current->point->vpoint, 2);
                currentM++;
                if (bestPointsDists.size()==N) //if set is full
                {
                    //if the biggest distance in set is equal/greater than current distance
                    if (prev(bestPointsDists.end())->second >= dist)
                    {
                        bestPointsDists.erase(prev(bestPointsDists.end())); //pop biggest distance pair
                        bestPointsDists.insert(make_pair(*(current->point),dist)); //insert new point/distance
                    }
                }
                else if (bestPointsDists.size()<N) //if there is space in set insert pair
                {
                    bestPointsDists.insert(make_pair(*(current->point),dist));
                }
                if (currentM == M) //maximum points to check reached
                {
                    high_resolution_clock::time_point end = high_resolution_clock::now();
                    duration<double> time_span = duration_cast<duration<double>>(end - start);
                    time = time_span.count();
                    return bestPointsDists;
                }   
            }

            currentProbes--;
            // If no more probes to check return.
            if (currentProbes == 0)
            {
                high_resolution_clock::time_point end = high_resolution_clock::now();
                duration<double> time_span = duration_cast<duration<double>>(end - start);
                time = time_span.count();
                return bestPointsDists;
            }
        }
        HDhasProbes.clear(); //clear table to get next hd's probes

    }
    // Max HD reached.
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end - start);
    time = time_span.count();
    return bestPointsDists;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::unordered_map<int,double> cube_approximate_range_search(Point q, double R, CubeTable cubeTable, CUBE_hash_info *hInfo)
{
    // Initialise an unordered map two hold points-distances inside radius r.
    unordered_map<int,double> rPoints;

    // Update hinfo with the right vectors for hash table, to compute query's g-value
    hInfo->update_v(cubeTable.v);
    hInfo->update_t(cubeTable.t);
    // Find g value for query point.
    vector<int> hValues;
    int k = hInfo->get_k();
    vector<int> vp = q.vpoint;
    for (int i = 0; i < k; i++)
    {
        hValues.push_back(compute_hValue(i, vp, hInfo));
    }
    vector<int> fValues;
    for (int i = 0; i < k; i++)
    {
        fValues.push_back(compute_fValue(i, hValues[i], hInfo));
    }
    int g = compute_gValue(fValues, hInfo);

    int M = hInfo->get_M(); //maximum number of points to search
    int currentM = 0; //keep track how many points we have searched till now

    // We finished checking vertice g but we have more points to check (because currentM < M).
    // We can't check more than 'probes minus the g' vertices and we will search until hamming distance = maxHD.
    int maxProbes = hInfo->get_probes();
    int maxHD = hInfo->maxHD;
    int numVertices = cubeTable.get_bucketsNumber();
    int currentProbes = maxProbes; //current probes will be reduced every time a new vertex is checked
    
    // Loop hamming distances till maximum hd defined in cube program.
    for (int hd = 0; hd <= maxHD; hd++) 
    {
        vector<int> HDhasProbes; //will consist of all vertices-indexes of current hamming distance
        
        // Loop all vertices and store only the ones with hamming distance = hd from g-index.
        for (int i = 0; i < numVertices; i++)
        {
            if(hammingDistance(g,i) == hd)
            {
                HDhasProbes.push_back(i);
            }
        }
        random_shuffle(HDhasProbes.begin(), HDhasProbes.end()); //randomize vertices
        
        // Loop from 0 to maxProbes and access current hd's probes.
        for (int i = 0; i < maxProbes; i++)
        {
            if (i==HDhasProbes.size()) //current hd has no more probes - break and increment hd
            {
                break;
            }
                        
            // Finally search vertice for best distance.
            list<Vertice> listToSearch = cubeTable.get_bucketList(HDhasProbes[i]);
            typename list<Vertice>::iterator current;
            for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
                double dist = distance(q.vpoint,current->point->vpoint, 2);
                currentM++;
                for (current = listToSearch.begin() ; current != listToSearch.end() ; ++current ) {
                    double dist = distance(q.vpoint,current->point->vpoint, 2);
                    if (dist < R && dist !=0)
                    {
                        rPoints.insert(make_pair(current->point->itemID,dist));
                    }
                }
                if (currentM == M) //maximum points to check reached
                {
                    return rPoints;
                }   
            }

            currentProbes--;
            // If no more probes to check return.
            if (currentProbes == 0)
            {
                return rPoints;
            }
        }
        HDhasProbes.clear(); //clear table to get next hd's probes

    }
    // Max HD reached.
    return rPoints;
}