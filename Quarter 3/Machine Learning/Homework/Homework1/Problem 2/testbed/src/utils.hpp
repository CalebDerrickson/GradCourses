#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
#include <algorithm>


static int pickRandomWeightedIndex(const std::vector<double>& weights);
static bool Convergence(std::vector<int>& prevAssignment, std::vector<int>& currAssignment);
static std::vector<double> vectorAdd(std::vector<double>& A, std::vector<double>& B);
static std::vector<double> vectorScalarMult(std::vector<double>&A, double a);


int pickRandomWeightedIndex(const std::vector<double>& weights) {
    // Calculate the cumulative sum of weights
    std::vector<double> cumulativeSum(weights.size());
    std::partial_sum(weights.begin(), weights.end(), cumulativeSum.begin());

    // Generate a random number within the total sum of weights
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, cumulativeSum.back());
    double randomNum = dis(gen);

    // Find the index where the random number falls
    auto it = std::upper_bound(cumulativeSum.begin(), cumulativeSum.end(), randomNum);
    return std::distance(cumulativeSum.begin(), it);
}


std::vector<double> vectorScalarMult(std::vector<double>&A, double a) {


    std::vector<double> B(A.size(), 0);

    for (int i = 0; i < A.size(); i++) {
        B[i] = a*A[i];
    }

    return B;
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
