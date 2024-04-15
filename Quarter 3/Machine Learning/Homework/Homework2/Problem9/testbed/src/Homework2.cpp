#include "Homework2.hpp"
#include "utils.hpp"
#include <iostream>

Homework2::Homework2(int numRuns, double fraction)
{
    // Reading in data
    std::cout<<"Reading in ../data/train35.digits"<<std::endl;
    _fileManager = FileManager<int>("../data/train35.digits", "");
    _fileManager.readData(' ', fraction);

    
    std::vector<std::vector<int>>* trainData = new std::vector<std::vector<int>>(_fileManager.getData());

    const int trainSize = trainData->size();
    const int trainSizeele = (*trainData)[0].size();
    std::cout<<"trainSize: "<<trainSize<<std::endl;
    std::cout<<"trainSizeele: "<<trainSizeele<<std::endl;
    

    std::vector<double> zero_vect(trainSizeele, 0); 


    _fileManager.resetData();
    
    std::cout<<"Reading in ../data/test35.digits"<<std::endl;
    _fileManager.setInPath("../data/test35.digits");
    _fileManager.readData(' ');

    std::vector<std::vector<int>>* testData = new std::vector<std::vector<int>>(_fileManager.getData());

    _fileManager.resetData();

    std::cout<<"Reading in ../data/train35.labels"<<std::endl;
    _fileManager.setInPath("../data/train35.labels");
    _fileManager.readData(' ', fraction);
    std::vector<std::vector<int>> temp = _fileManager.getData();
    
    // Prediction labels read in as array. Form into vector.
    const int size = temp.size();
    std::vector<int>* predictionLabels = new std::vector<int>(size, 0);
     
    for (int i = 0; i < size; i++) {
        (*predictionLabels)[i] = temp[i][0];
    }

    _perceptron = new Perceptron(testData, trainData, predictionLabels);
    _numRuns = numRuns;
}

Homework2::~Homework2()
{
    delete _perceptron;
}

int Homework2::run()
{
    std::cout<<"Running Perceptron..."<<std::endl;

    if ((*_perceptron).run(_numRuns) != 0) {
        std::cout<<"Error"<<std::endl;
        return 1;
    }

    std::cout<<"Writing accuracies to ../data/accuracies.csv"<<std::endl;

    FileManager<double> accReadOut("", "../data/accuracies.csv");
    std::vector<double> accData = (*_perceptron).getAccuracies();
    std::vector<std::vector<double>> accDatawrite(1, accData);
    accReadOut.setData(accDatawrite);
    accReadOut.writeData(',');

    return 0;
}

