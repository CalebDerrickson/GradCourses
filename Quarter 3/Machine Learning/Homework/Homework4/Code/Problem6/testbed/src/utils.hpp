#pragma once
#include <math.h>
#include "logdouble.hpp"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>


typedef std::vector<std::vector<double>> mat_dbl;
typedef std::vector<std::vector<int>> mat_int;
typedef std::vector<std::vector<logdouble>> mat_logdouble;

typedef enum state {GOOD, EXIT_BAD, EXIT_GOOD} state;

namespace utils{

// Function to generate a random matrix with given mean
inline std::vector<std::vector<double>> generateRandomMatrix(int rows, int cols, double mean) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));

    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(mean, mean / 5.); // Adjust standard deviation as needed

    // Generate random numbers and fill the matrix
    for (int i = 0; i < rows; ++i) {
        double sum = 0.0;
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = std::abs(dist(gen));
            sum += matrix[i][j];
        }
        // Normalize row to ensure sum equals 1
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] /= sum;
        }
    }

    return matrix;
}

// Function to generate a random vector with given size and mean
inline std::vector<double> generateRandomVector(int size, double mean) {
    std::vector<double> vec(size);

    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(mean, mean / 5.); // Adjust standard deviation as needed

    // Generate random numbers and fill the vector
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        vec[i] = std::abs(dist(gen));
        sum += vec[i];
    }

    // Normalize vector to ensure sum equals 1
    for (int i = 0; i < size; ++i) {
        vec[i] /= sum;
    }

    return vec;
}



}