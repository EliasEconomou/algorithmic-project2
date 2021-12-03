#include <iostream>
#include <algorithm>
#include <string>
#include <getopt.h>
#include <vector>
#include "../../include/point_functions.hpp"
#include "../../include/hash_table.hpp"
#include "../../include/hash_functions.hpp"
#include "../../include/algorithms.hpp"


using namespace std;


int main(int argc, char** argv) {

    string inputFile, queryFile, outputFile, algorithm, metric="NULL";
    int k=-1; //k will get a default value later according to algorithm given
    int L=5, M=10, probes=2;
    double delta=0.0;
    string word;

    //---Parse arguments---
    for (int i = 1; i < argc; i++){
        word  = argv[i];
        if(word == "-i"){
            if(i+1 < argc){
                inputFile = argv[++i];
            }else{
                cout << "No argument for input file." << endl;
                return -1;
            }
        }
        else if (word == "-q"){
            if(i+1 <= argc){
                queryFile = argv[++i];
            }else{
                cout << "No argument for configuration file." << endl;
                return -1;
            }
        }
        else if(word == "-k"){
            if(i+1 <= argc){
                k = stoi(argv[++i]);
            }else{
                cout << "No argument for k value." << endl;
                return -1;
            }
        }
        else if(word == "-L"){
            if(i+1 <= argc){
                L = stoi(argv[++i]);
            }else{
                cout << "No argument for L value." << endl;
                return -1;
            }
        }
        else if(word == "-M"){
            if(i+1 <= argc){
                M = stoi(argv[++i]);
            }else{
                cout << "No argument for M value." << endl;
                return -1;
            }
        }
        else if(word == "-probes"){
            if(i+1 <= argc){
                probes = stoi(argv[++i]);
            }else{
                cout << "No argument for probes value." << endl;
                return -1;
            }
        }
        else if (word == "-o"){
            if(i+1 <= argc){
                outputFile = argv[++i];
            }else{
                cout << "No argument for output file." << endl;
                return -1;
            }
        }
        else if (word == "-algorithm"){
            if(i+1 <= argc){
                algorithm = argv[++i];
                if ((algorithm!="LSH") && (algorithm!="Hypercube") && (algorithm!="Frechet"))
                {
                    cout << "Wrong algorithm argument given." << endl;
                    return -1;
                }
                
            }else{
                cout << "No argument for algorithm method." << endl;
                return -1;
            }
        }
        else if(word == "-metric"){
            if(i+1 <= argc){
                metric = argv[++i];
                if ((metric!="discrete") && (metric!="continuous"))
                {
                    cout << "Wrong metric argument given." << endl;
                    return -1;
                }
            }
        }
        else if(word == "-delta"){
            if(i+1 <= argc){
                delta = stod(argv[++i]);
            }else{
                cout << "No argument for delta value." << endl;
                return -1;
            }
        }
        else{

            cout << "Error in arguments." << " Wrong argument: " << word << endl;
            return -1;
        }
    }
    if ((algorithm == "Frechet") && (metric == "NULL")) //Frechet must be discrete/continuous
    {
        cout << "No metric for Frechet given." << endl;
        return -1;
    }
    if (((algorithm == "LSH") || (algorithm == "Hypercube")) && (metric != "NULL"))
    {
        cout << "LSH/Hypercube can't have Frechet metric." << endl;
        return -1;
    }
    if (k==-1) //k not given - use default
    {
        if (algorithm=="LSH")
            k=4;
        else if (algorithm=="Hypercube")
            k=14;
    }
    
    // cout << "inputFile = " << inputFile << ". queryFile = " << queryFile << ". k = " << k << ". L = " << L;
    // cout << ". M = " << M << ". probes = " << probes << ". outputFile = " << outputFile << ". algorithm = " << algorithm;
    // cout << ". metric = " << metric << ". delta = " << delta << "." << endl;


}