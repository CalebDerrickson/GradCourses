#include "GaussianMixture.hpp"
#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <random>
#include <numeric>
#include <algorithm>
#include <ctime>


# define MY_PI          3.141592653589793238462643383279502884L /* pi */
// Private funcitons
// bool Convergence(std::vector<int>& prevAssignment, std::vector<int>& currAssignment);
// std::vector<double> vectorAdd(std::vector<double>& A, std::vector<double>& B);
// std::vector<double> vectorScalarMult(std::vector<double>&A, double a);
void getMeanAndVariance(std::vector<double>* point, double &mean, double &variance);
void outerProduct (std::vector<double> &vec1, std::vector<double> &vec2, Sigma* sig);
std::vector<double> vectorDifference(std::vector<double>& vec1, std::vector<double>& vec2);
void matrixAdd(std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& B, std::vector<std::vector<double>>& C);
void matrixScale(std::vector<std::vector<double>> &A, double scalar, std::vector<std::vector<double>>& B);
double mathcalN(std::vector<double> &x, std::vector<double>& mu, std::vector<std::vector<double>> &Sig);
void innerProduct(std::vector<double>& vec1, std::vector<double>& vec2, double &val);
// int pickRandomWeightedIndex(const std::vector<double>& weights);
void matVecMult(std::vector<std::vector<double>> &mat, std::vector<double> &vec, std::vector<double> &out);

GaussianMixture::GaussianMixture(const char* filePath, const char* outPath, const int k) :
    _data({}),
    _outData({}),
    _filePath(filePath),
    _outPath(outPath),
    _numClusters(k)
{
    _clusterPriors.resize(k);
}

GaussianMixture::~GaussianMixture()
{

}

int GaussianMixture::Run()
{

    
    //Read in data
    std::cout<<"Reading in data from file path " <<_filePath<<" ..."<<std::endl;
    
    if (ReadInData() == 0) {
        std::cout<<"Data not read from " <<_filePath<< "!"<<std::endl;
        return 0;
    }

    // Run GaussianMixtureCalc()
    if (GaussianMixtureCalc() == 0) {
        std::cout<<"Error in GaussianMixtureCalc!" << std::endl;
        return 0;
    }

    // std::cout<<"Writing data to outputCluster.txt";
    if (WriteOutData() == 0) {
        std::cout<<"Error in write out data."<<std::endl;
    }

    return 1;
}

int GaussianMixture::ReadInData()
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

int GaussianMixture::WriteOutData()
{
    std::ofstream OutFile("outputGaussian.txt", std::ios::trunc);
    
    if(!OutFile.is_open()) {
        return 0;
    }
    
    for (int i = 0; i < _data.size(); i++) {
        std::string line{};
        for (int j = 0; j < _data[0].size(); j++) {
            line.append(std::to_string(_data[i][j]));
            line.push_back(',');
        }
        line.append(std::to_string(_gaussianAssignment[i]));
        line.append("\n");
        OutFile << line;
    }
    
    OutFile.close();
    
    std::ofstream OutFile1(_outPath, std::ios::app);
    if(!OutFile1.is_open()) {
        return 0;
    }

    for (int i = 0; i < _logLikelihood.size(); i++) {
        OutFile1 << std::to_string(_logLikelihood[i]) << ',';
    }
    OutFile1 << '\n';
    OutFile1.close();

    return 1;
}

std::vector<double> GaussianMixture::ParseLine(std::string& row) 
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


void GaussianMixture::PrintData()
{
    for(std::vector<double>& row : _data) {
        for (double& ele : row) {
            std::cout<< ele << " ";
        }
        std::cout<<std::endl;
    }
}



int GaussianMixture::InitGaussians()
{
    // Init Cluster priors
    double initClusterPriorVal = initClusterPriorVal = 1.0/(double)_numClusters;
    
    _clusterPriors.resize(_numClusters, initClusterPriorVal);
    
    for (int k = 0; k < _numClusters; k++) {
        _clusterPriors[k] = initClusterPriorVal;

    }
    // Get Means

    // Initialize rng
    const int datasize = _data[0].size();
    std::uniform_real_distribution<double> unif(0, datasize);
    std::default_random_engine re;
    re.seed(time(0));

    _clusterMean.resize(_numClusters);
    for (int i = 0; i < _numClusters; i++) {
        _clusterMean[i] = _data[unif(re)];
    }

    // // Init Covariances

    // Idk Random it.
    std::uniform_real_distribution<double> unif1(-1.0, 1.0);
    std::default_random_engine re1;

    _clusterCovariance.resize(_numClusters);

    for (int j = 0; j < _numClusters; j++) {
        _clusterCovariance[j].values.resize(datasize);
        for (int i = 0; i < datasize; i++) {
            _clusterCovariance[j].values[i].resize(datasize);
            for (int k = 0; k < datasize; k++) {
                _clusterCovariance[j].values[i][k] = unif1(re1);
            }
        }
    }

    // _clusterCovariance.resize(_numClusters);
    // for (int j = 0; j < _numClusters; j++) {
    //     _clusterCovariance[j].values.resize(datasize*datasize);
    //     std::vector<std::vector<double>> sum(datasize, std::vector<double>(datasize));
    //     for (int i = 0; i < datasize; i++) {
    //         
    //         // Calculate outerProduct
    //         std::vector<double> diff = vectorDifference(_data[i], _clusterMean[j]);
    //         Sigma temp{};
    //         outerProduct(diff, diff, &temp);
    //
    //         // Sum over outerProducts
    //         matrixAdd(sum, temp.values, sum);
    //     }
    //     // Average OuterProducts
    //     matrixScale(sum, 1.0/datasize, _clusterCovariance[j].values);
    //     _clusterCovariance[j].numRows = sum.size();
    //     _clusterCovariance[j].NumColumns = sum[0].size();
    // 
    // for (int k = 0; k < datasize; k++) {
    //     for (int l = 0; l < datasize; l++) {
    //         std::cout<<sum[k][l] << ' ';
    //     }
    //     std::cout<<std::endl;
    // }
    // 
    // }
     
    return 1;
}

int GaussianMixture::GaussianMixtureCalc()
{

    // Initialize parameters
    if (!InitGaussians()) {
        std::cout<<"Error in Gaussian Init!"<<std::endl;
        return 0;
    }

    const int MAX_ITER = 200;

    // Initialize cluster assignments to -1
    if (_gaussianAssignment.size() != _data.size()) {
        _gaussianAssignment.resize(_data.size(), 0);
    }
    for (int i = 0; i < _data.size(); i++) {
        _gaussianAssignment[i] = -1;
    }

    std::vector<int> prevAssignment = _gaussianAssignment;

    for (int iter = 0; iter < MAX_ITER; iter++)
    {


        // Update cluster priors
        updateClusterPriors();

        // Update Cluster Means
        updateClusterMeans();

        // Update Cluster covariances
        updateClusterCovariance();

        // Assign points to clusters
        AssignPointstoClusters();    
    
        // Stopping criterion
        if(Convergence(prevAssignment, _gaussianAssignment)) {
            break;
        }

        _logLikelihood.push_back(calculateLogLikelihood());

        // Haven't gotten to minimum, keep going
        prevAssignment = _gaussianAssignment;
    }

    return 1;
}


void GaussianMixture::updateClusterPriors()
{
    const int numDataPoints = _data.size();
    
    for (int j = 0; j < _numClusters; j++) {
        double sum = 0;
        for (int i = 0; i < numDataPoints; i++) {
            sum += computePij(i, j);
        }
    
        _clusterPriors[j] = sum / (double)numDataPoints;
    }   
}

void GaussianMixture::updateClusterMeans()
{
    const int numDataPoints = _data.size();
    const int datasize = _data[0].size();

    for (int j = 0; j < _numClusters; j++) {
        std::vector<double> num(datasize, 0);
        double den = 0;
        for (int i = 0; i < numDataPoints; i++) {
            std::vector<double> temp = vectorScalarMult(_data[j], computePij(i, j));
            num = vectorAdd(num, temp);
            den += computePij(i, j);
        }
        _clusterMean[j] = vectorScalarMult(num, 1.0 / den);
    }
}

void GaussianMixture::updateClusterCovariance()
{

    const int numDataPoints = _data.size();
    const int datasize = _data[0].size();
    
    std::vector<std::vector<double>> zeroMatrix(datasize, std::vector<double>(datasize));
    for (int j = 0; j < _numClusters; j++) {
        std::vector<std::vector<double>> num(datasize, std::vector<double>(datasize));
        double den = 0;

        for (int i = 0; i < numDataPoints; i++) {
            double pij = computePij(i, j);
            std::vector<double> diff = vectorDifference(_data[i], _clusterMean[j]);
            Sigma sig = { .numRows = datasize, .NumColumns = datasize, };
            sig.values = zeroMatrix;
            outerProduct(diff, diff, &sig);
            std::vector<std::vector<double>> temp(datasize, std::vector<double>(datasize));
            matrixScale(sig.values, pij, temp);
            matrixAdd(num, temp, num);
            den += computePij(i, j);
        }

        matrixScale(num, 1.0 / den, _clusterCovariance[j].values);
    }
}


double GaussianMixture::computePij(const int i, const int j)
{
    double pij = 0;

    double temp = mathcalN(_data[i], _clusterMean[j], _clusterCovariance[j].values);
    pij = _clusterPriors[j] * temp;
    double sum = 0;
    for (int l = 0; l < _numClusters; l++) {
        sum += _clusterPriors[l] * mathcalN(_data[i], _clusterMean[l], _clusterCovariance[l].values); 
    }
    pij = pij / sum;
    return pij;
}

void GaussianMixture::AssignPointstoClusters()
{


    const int numDataPoints = _data.size();
    const int dataPointSize = _data[0].size();

    if (_gaussianAssignment.size() != numDataPoints) {
        _gaussianAssignment.resize(numDataPoints);
    }


    // Inverting covariance -> only a 2by2

    // Assign points based on the max value of pij 
    // Involves computing mathcalN for all i for all j


    for (int i = 0; i < numDataPoints; i++) {
        
        // Get pij for all data points 
        std::vector<double> clusterprobs(_numClusters, 0);
        for (int j = 0; j < _numClusters; j++) {

            // Compute pij
            double temp = mathcalN(_data[i], _clusterMean[j], _clusterCovariance[j].values);
            clusterprobs[j] = _clusterPriors[j] * temp;
            double sum = 0;
            for (int l = 0; l < _numClusters; l++) {
                sum += _clusterPriors[l] *mathcalN(_data[i], _clusterMean[l], _clusterCovariance[l].values);
            }

            clusterprobs[j] = clusterprobs[j] / sum;
        }
        _gaussianAssignment[i] = pickRandomWeightedIndex(clusterprobs);
    }

}

double GaussianMixture::calculateLogLikelihood()
{
    const int numDataPoints = _data.size();

    double outersum = 0;
    for (int i = 1; i < numDataPoints; i++) {
        double sum = 0;
        for (int j = 0; j < _numClusters; j++) {
            sum += _clusterPriors[j] * mathcalN(_data[i], _clusterMean[j], _clusterCovariance[j].values);
        }
        outersum += std::log1p(sum);
    }
    return outersum;
}



// PRIVATE FUNCTIONS

// bool Convergence(std::vector<int>& prevAssignment, std::vector<int>& currAssignment) 
// {
//     return (prevAssignment == currAssignment);
// }

// std::vector<double> vectorAdd(std::vector<double>& A, std::vector<double>& B) {
//     std::vector<double> C(A.size(), 0);
// 
//     for (int i = 0; i < A.size(); i++) {
//         C[i] = A[i] + B[i];
//     }
// 
//     return C;
// }

// std::vector<double> vectorScalarMult(std::vector<double>&A, double a) {
// 
// 
//     std::vector<double> B(A.size(), 0);
// 
//     for (int i = 0; i < A.size(); i++) {
//         B[i] = a*A[i];
//     }
// 
//     return B;
// }

void getMeanAndVariance(std::vector<double>* point, double &mean, double &variance)
{
    // Get Mean
    double sum = 0;
    int size = (*point).size();
    for (int i = 0; i < size; i++) {
        sum += (*point)[i];
    }
    mean = sum / size;

    // Get Variance
    sum = 0; 
    for (int i = 0; i <  size; i++) {
        sum += std::pow((*point)[i] - mean, 2.0);
    }
    variance = sum / size;
}

void outerProduct (std::vector<double> &vec1, std::vector<double> &vec2, Sigma* sig)
{

    // rows of multiples of vec2

    (*sig).values.resize(vec1.size());
    (*sig).numRows = vec1.size();
    (*sig).NumColumns = vec2.size();

    for (int i = 0; i < vec1.size(); i++) {
        (*sig).values[i].resize((*sig).NumColumns);
        (*sig).values[i] = vectorScalarMult(vec2, vec1[i]);
    }
}

std::vector<double> vectorDifference(std::vector<double>& vec1, std::vector<double>& vec2)
{
    const int size = vec1.size();
    std::vector<double> vec3(size, 0);
    for (int i = 0; i < size; i++) {
        vec3[i] = vec1[i] - vec2[i];
    }

    return vec3;
}


void matrixAdd(std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& B, std::vector<std::vector<double>>& C)
{

    C.resize(A.size());

    for (int i = 0; i < A.size(); i++) {
        C[i].resize(A[i].size());
        for (int j = 0; j < A[i].size(); j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrixScale(std::vector<std::vector<double>> &A, double scalar, std::vector<std::vector<double>>& B)
{
    int numRows = A.size();
    int numCols = A[0].size();
    B.resize(numRows);
    for (int i = 0; i < numRows; i++) {
        B[i].resize(numCols);
        for (int j = 0; j < numCols; j++){
            B[i][j] = A[i][j] * scalar;
        }
    }
}

std::vector<std::vector<double>> SigInverse(std::vector<std::vector<double>> &sig)
{

    std::vector<std::vector<double>> out{};
    out.resize(sig.size());

    for (int i = 0; i < sig.size(); i++) {
        out[i].resize(sig[i].size());
    }

    // Calculate det, is 2by2
    double det = sig[0][0] * sig[1][1] - sig[0][1] * sig[1][0];
    det = 1.0 / det;

    out[0][0] = det*sig[1][1];
    out[1][1] = det*sig[0][0];
    out[0][1] = -det*sig[0][1];
    out[1][0] = -det*sig[1][0];
    return out;

}

void matVecMult(std::vector<std::vector<double>> &mat, std::vector<double> &vec, std::vector<double> &out)
{
    out.resize(vec.size());
    const int numRows = mat.size();
    const int numCols = mat[0].size();

    for (int i = 0; i < numRows; i++) {
        double sum = 0;
        for (int j = 0; j < numCols; j++) {
            sum += mat[i][j] * vec[j];
        }
        out[i] = sum;
    }

}

double mathcalN(std::vector<double> &x, std::vector<double>& mu, std::vector<std::vector<double>> &Sig)
{
    // Break up into terms
    const int d = x.size();

    double term1 = std::pow(2*MY_PI, -d / 2.0);

    // Sigma is 2by2 
    double det = Sig[0][0] * Sig[1][1] - Sig[0][1] * Sig[1][0];
    det = std::sqrt(std::abs(det));
    double term2 = 1.0 / det;

    std::vector<std::vector<double>> inv = SigInverse(Sig);
    
    std::vector<double> diff = vectorDifference(x ,mu);

    std::vector<double> asp2(d, 0);
    matVecMult(inv, diff, asp2);
    

    double asp = 0;
    innerProduct(diff, asp2, asp);
    asp = -asp/2.0;

    double term3 = expf(asp);

    return term1*term2*term3;

}


void innerProduct(std::vector<double>& vec1, std::vector<double>& vec2, double &val)
{
    const int vecsize = vec1.size();

    for (int i = i; i < vecsize; i++) {
        val += vec1[i] * vec2[i];
    }

}

// int pickRandomWeightedIndex(const std::vector<double>& weights) {
//     // Calculate the cumulative sum of weights
//     std::vector<double> cumulativeSum(weights.size());
//     std::partial_sum(weights.begin(), weights.end(), cumulativeSum.begin());
// 
//     // Generate a random number within the total sum of weights
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<double> dis(0.0, cumulativeSum.back());
//     double randomNum = dis(gen);
// 
//     // Find the index where the random number falls
//     auto it = std::upper_bound(cumulativeSum.begin(), cumulativeSum.end(), randomNum);
//     return std::distance(cumulativeSum.begin(), it);
// }
