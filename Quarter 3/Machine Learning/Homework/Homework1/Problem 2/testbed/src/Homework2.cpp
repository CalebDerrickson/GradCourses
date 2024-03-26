#include "Homework2.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <random>
#include <algorithm>
#include <ctime>


Homework2::Homework2(const char* filePath, const int k) :
    _data({}),
    _outData({}),
    _filePath(filePath),
    _numClusters(k)
{

}

Homework2::~Homework2()
{

}

int Homework2::Run()
{

    
    //Read in data
    std::cout<<"Reading in data from file path " <<_filePath<<" ..."<<std::endl;
    
    if (ReadInData() == 0) {
        std::cout<<"Data not read from " <<_filePath<< "!"<<std::endl;
        return 0;
    }

    // Print data (for debugging)
    // std::cout<<"Printing Input Data"<<std::endl;
    // PrintData();

    // Run KMeans()
    if (KMeans() == 0) {
        std::cout<<"Error in KMeans!" << std::endl;
        return 0;
    }

    // std::cout<<"K Means Finished. Cluster Assignments:" <<std::endl;
    // std::cout<<"Point \tCluster assignment"<<std::endl;
    // for (int i = 0; i < _data.size(); i++) {
    //     std::cout<< i+1 << '\t' << _clusterAssignment[i] << std::endl; 
    // }

    // std::cout<<"Writing data to outputCluster.txt";
    if (WriteOutData() == 0) {
        std::cout<<"Error in write out data."<<std::endl;
    }

    return 1;
}

int Homework2::ReadInData()
{
    std::string row;
    std::ifstream readFile;
    readFile.open(_filePath);

    if (!readFile.is_open()) {
        std::cout<<"File path " << _filePath <<" is not defined!"<<std::endl;
        return 0;
    }
    
    while(std::getline(readFile, row)) {
        // parse row 
        std::vector<double> parsedLine = ParseLine(row); 
        _data.push_back(parsedLine);
    }

    readFile.close();
    return 1;
}

int Homework2::WriteOutData()
{
    // Commenting out for the 20 times run
    // std::ofstream OutFile("outputCluster.txt", std::ios::trunc);
    // 
    // if(!OutFile.is_open()) {
    //     return 0;
    // }
    // 
    // for (int i = 0; i < _data.size(); i++) {
    //     std::string line{};
    //     for (int j = 0; j < _data[0].size(); j++) {
    //         line.append(std::to_string(_data[i][j]));
    //         line.push_back(',');
    //     }
    //     line.append(std::to_string(_clusterAssignment[i]));
    //     line.append("\n");
    //     OutFile << line;
    // }
    // 
    // OutFile.close();
    
    //Change to std::ios::trunc after 20 times run
    std::ofstream OutFile1("outputDistortion.txt", std::ios::app);
    if(!OutFile1.is_open()) {
        return 0;
    }

    for (int i = 0; i < _distortion.size(); i++) {
        OutFile1 << std::to_string(_distortion[i]) << ',';
    }
    OutFile1 << '\n';
    OutFile1.close();

    return 1;
}

std::vector<double> Homework2::ParseLine(std::string& row) 
{

    std::vector<double> res = {};
    char delimiter = ',';
    int prevIndex = 0;


    for (int i = 0; i < row.size(); i++) {
        char c = row.data()[i];

        if (c == delimiter || i == row.size() - 1) {
            double val = 0;
            std::string temp{};
            if (prevIndex == 0) {
                temp = row.substr(prevIndex, i - prevIndex);
            }
            else {
                temp = row.substr(prevIndex+1, i - prevIndex);
            }
            val = std::stod(temp);            
            res.push_back(val);
            prevIndex = i;
        }

    }

    return res;
}


void Homework2::PrintData()
{
    for(std::vector<double>& row : _data) {
        for (double& ele : row) {
            std::cout<< ele << " ";
        }
        std::cout<<std::endl;
    }
}

double distance(const std::vector<double>& point, const std::vector<double>& cluster)
{
    double res = 0;
    for (int i = 0; i < point.size(); i++) {
        res += std::pow(point[i] - cluster[i], 2);
    }

    res = std::pow(res, 0.5);
    return res;
}


void Homework2::InitClusters()
{
    // Getting the min and max values for float distribution
    double minVal = INFINITY;
    double maxVal = -INFINITY;

    for (std::vector<double>& row : _data) {
        for (double& ele : row) {
            if (ele < minVal) {
                minVal = ele;
            }
            if (ele > maxVal) {
                maxVal = ele;
            }
        }
    }

    // Initialize rng
    std::uniform_real_distribution<double> unif(minVal, maxVal);
    std::default_random_engine re;
    re.seed(time(0));

    // Assign random values to centroids
    std::vector<double> cluster(_data[0].size(), 0);
    for(int i = 0; i < _numClusters; i++) {    
        for (int j = 0; j < cluster.size(); j++) {
            cluster[j] = unif(re);
        }
        _clusterCentroids.push_back(cluster);
    }
}

void Homework2::CalculateDistortion()
{
    // This is J_avg^2
    
    std::vector<double> sumDistanceFromCentroid(_numClusters, 0);

    // Sum over Clusters
    for (int i = 0; i < _numClusters; i++) {
    // Sum over points assigned to clusters their distance from centroid

        for (int j = 0; j < _clusterAssignment.size(); j++) {
            if (_clusterAssignment[j] == i) {
                sumDistanceFromCentroid[i] += std::pow(distance(_data[j], _clusterCentroids[i]), 2.0);
            }
        }
    }
    
    double sum = 0;
    for (int i = 0; i < _numClusters; i++) {
        sum += sumDistanceFromCentroid[i];
    }

    _distortion.push_back(sum);
}

bool Convergence(std::vector<int>& prevAssignment, std::vector<int>& currAssignment) 
{
    return (prevAssignment == currAssignment);
}

std::vector<double> vectorAdd(std::vector<double>& A, std::vector<double>& B) {
    std::vector<double> C(A.size(), 0);

    for (int i = 0; i < A.size(); i++) {
        C[i] = A[i] + B[i];
    }

    return C;
}

std::vector<double> vectorScalarMult(std::vector<double>&A, double a) {
    std::vector<double> B(A.size(), 0);

    for (int i = 0; i < A.size(); i++) {
        B[i] = a*A[i];
    }

    return B;
}

int Homework2::KMeans()
{
    if (_numClusters == 0) {
        std::cout<< "Number of Expected clusters must be greater than zero." << std::endl;
        return 0;
    }

    // Assign random values to centroids
    InitClusters();
    std::cout<<"Assigned " <<_clusterCentroids.size() << " cluster centroids."<<std::endl;

    // Initialize cluster assignments
    for (int i = 0; i < _data.size(); i++) {
        _clusterAssignment.push_back(-1);
    }

    int numDataPoints = _data.size();
    std::vector<int> prevAssignment = _clusterAssignment;
    std::vector<double> distanceToCentroid(_numClusters, 0);
            
    int max_iter = 200;
    
    for(int i = 0; i < max_iter; i++) {

        // Assign each point to the closest center
        for (int j = 0; j < numDataPoints; j++) {

            std::vector<double> distanceToCentroid(_numClusters, 0);
            // Get Distances
            for (int k = 0; k < _numClusters; k++) {
                distanceToCentroid[k] = distance(_data[j], _clusterCentroids[k]);
            }
            
            // Get smallest distance
            int newAssignment = std::min_element(distanceToCentroid.begin(), distanceToCentroid.end()) - distanceToCentroid.begin();

            // Assign point to new cluster
            _clusterAssignment[j] = newAssignment;
        }

        // Recompute cluster centers
        for (int j = 0; j < _numClusters; j++) {
            int nPointsInCluster = 0;
            std::vector<double> newCentroid(_clusterCentroids[j].size(), 0);
            
            for (int k = 0; k < numDataPoints; k++) {
                if (_clusterAssignment[k] == j) {
                    // Point is assigned to cluster, add it.
                    newCentroid = vectorAdd(newCentroid, _data[k]);
                    nPointsInCluster++;
                }
            }

            // Average it.
            if (nPointsInCluster == 0) {
                std::cout<<"Error"<<std::endl;
                _clusterCentroids[j] = newCentroid;
                continue;
            }

            newCentroid = vectorScalarMult(newCentroid, (1.0 / nPointsInCluster));
            _clusterCentroids[j] = newCentroid;
        }


        if (Convergence(prevAssignment, _clusterAssignment) && i > 0) {
            return 1;
        }

        // Calculate Distortion
        CalculateDistortion();
        int temp = _distortion.size();

        // std::cout<<"Distortion Value: "<< _distortion[temp - 1]<<std::endl;
        // Have not converged (yet), reassign prevAssignment
        prevAssignment = _clusterAssignment;
    }

    return 1;
}

