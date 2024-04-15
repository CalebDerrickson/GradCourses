#include "Perceptron.hpp"
#include "utils.hpp"
#include <iostream>

Perceptron::Perceptron()
{

}

Perceptron::Perceptron(
        std::vector<std::vector<int>>* testData, 
        std::vector<std::vector<int>>* trainData, 
        std::vector<int>* predictionLabels
    )
{
    _testData = testData;
    _trainData = trainData;
    _predictionLabels = predictionLabels;
    

    int size = _testData->size();
    if (size == 0) {
        std::cout<<"ERROR: DATA SIZE IS ZERO"<<std::endl;
    }
    size = (*_testData)[0].size();


    // Initializing weights
    _weights = std::vector<double>(size, 0);
    _predictions = std::vector<int>(_testData->size(), 0);
}

Perceptron::~Perceptron()
{
    delete _testData;
    delete _trainData;
    delete _predictionLabels;
}

int Perceptron::run(int numRuns)
{
    _accuracy = std::vector<double>(numRuns, 0);

    // Train, then test. numRuns times
    for(int i = 0; i < numRuns; i++) {

        if (train(i) != 0) {
            std::cout<<"Error occured in training."<<std::endl;
            return 1;        
        } 
        std::cout<<"Percentage Done : " << (double) (i+1) / numRuns * 100.0f<<std::endl;
    }
    if (test() != 0) {
        std::cout<<"Error occured in testing."<<std::endl;
        return 1;
    }  

        
    return 0;   
}


void Perceptron::clearWeights()
{
    int size = _weights.size();
    for (int i = 0; i < size; i++) {
        _weights[i] = 0;
    }
}

int Perceptron::train(int numRun)
{
    // First train, then get accuracy
    int trainSize = _trainData->size();

    for (int i = 0; i < trainSize; i++) {
        
        // Prediction
        int prediction = 0;
        predict(prediction, (*_trainData)[i]);

        // Updating
        bool pred1 = (prediction == -1) && ((*_predictionLabels)[i] == 1);
        bool pred2 = (prediction == 1) && ((*_predictionLabels)[i] == -1);

        if (pred1) _weights = utils::vector_sum<double, int>(_weights, (*_trainData)[i]);

        if (pred2) _weights = utils::vector_diff<double, int>(_weights, (*_trainData)[i]);
        
    }

    int numCorrect = 0;
    for (int i = 0; i < trainSize; i++) {
        
        // Prediction
        int prediction = 0;
        predict(prediction, (*_trainData)[i]);

        if (prediction * (*_predictionLabels)[i] >= 0) numCorrect++; 
    }
    std::cout<<"Accuracy : "<<(double)numCorrect / (double)trainSize<<std::endl;
    _accuracy[numRun] = (double)numCorrect / (double)trainSize;
    return 0;
}

int Perceptron::test()
{
    int testSize = _testData->size();
    int numCorrect = 0;

    for (int i = 0; i < testSize; i++) {
        // Prediction
        int prediction = 0;
        predict(prediction, (*_testData)[i]);
        _predictions[i] = prediction;
    }
    return 0;
}

void Perceptron::predict(int &predict, std::vector<int>& predictVect)
{
    if (utils::dot_product<double, int>(_weights, predictVect) >= 0) predict = 1;
    else predict = -1;
}
