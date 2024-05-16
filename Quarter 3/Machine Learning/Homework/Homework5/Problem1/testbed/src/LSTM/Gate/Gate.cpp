#include "Gate.hpp"
#include "utils.hpp"
#include <iostream>
#include <stdio.h>
#include <random>

Gate::Gate()
{
    //Random Weights, bias
    // Activation function -> ReLU

  
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    Weights = mat_dbl(VECTOR_SIZE, vec_dbl(2*VECTOR_SIZE, 0.0));
    bias = vec_dbl(VECTOR_SIZE, 0.0);
    actFunc = utils::act_func::ReLU;
    
    for (int i = 0; i < VECTOR_SIZE; i++) {
        for (int j = 0; j < 2*VECTOR_SIZE; j++) {
            Weights[i][j] = dis(gen);
        }
        bias[i] = dis(gen);
    }

}

Gate::Gate(mat_dbl& weights, vec_dbl& bias, double (*actFunc)(double))
    : Weights(weights), bias(bias), actFunc(actFunc)
{


}

Gate::~Gate()
{


}

void Gate::resetWeights()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int i = 0; i < Weights.size(); i++) {
        for (int j = 0; j < Weights[i].size(); j++) {
            Weights[i][j] = dis(gen);
        }
    }

}

void Gate::resetBias()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int i = 0; i < bias.size(); i++) {
        bias[i] = dis(gen);
    }

}

void Gate::setActFunc(double (*newActFunc)(double))
{
    actFunc = newActFunc;

}

void Gate::updateWeights(mat_dbl& weights_)
{
    utils::checkMatrixDimensions(Weights);
    utils::checkMatrixDimensions(weights_);
    if (weights_.size() != Weights.size() || weights_[0].size() != Weights[0].size()) {
        printf("Gate::updateWeights: New weights matrix not of same size. Expected (row, column) : (%d, %d), Actual (row, column) : (%d, %d)",
           Weights.size(), Weights[0].size(), weights_.size(), weights_[0].size());
        return;
    }

    Weights = weights_;
}

void Gate::updateBias(vec_dbl& bias_)
{
    utils::checkVectorDimensions(bias);
    utils::checkVectorDimensions(bias_);
    if (bias.size() != bias_.size()) {
        printf("Gate::updateBias: New bias vector not of same size. Expected: %d, Actual: %d", bias.size(), bias_.size());
        return;
    }

    bias = bias_;
}

vec_dbl Gate::forward(vec_dbl& vec)
{

    vec_dbl forwardVec(vec.size(), 0.0);
    
    forwardVec = utils::MatVecMult(Weights, vec);
    
    forwardVec = utils::vecAdd(forwardVec, bias);
    
    for (int i = 0; i < forwardVec.size(); i++) {
        forwardVec[i] = actFunc(forwardVec[i]);
    }

    return forwardVec;
}
