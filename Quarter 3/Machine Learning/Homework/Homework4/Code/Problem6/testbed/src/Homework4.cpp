#include "Homework4.hpp"
#include "HMM.hpp"
#include <iostream>
#include <random>
#include <algorithm>



Homework4::Homework4(const char* inFilePath, const char* outFilePath)
{
    // Setting _inPath
    _inData = std::make_unique<FileManager<int>>();
    _inData->setInPath(inFilePath);

    // Setting _outPath
    _outData = std::make_unique<FileManager<double>>();
    _outData->setOutPath(outFilePath);

    _HMM = std::make_unique<HMM*>(new HMM);    
}

Homework4::~Homework4()
{

}

void Homework4::run()
{
    if (initHMM() != state::GOOD) return;
    (*_HMM)->run();
    collectData();
}

state Homework4::initHMM()
{
    std::cout<<"Reading in "<<_inData->getInPath()<<std::endl;
     _inData->readData(' ');
    
    // Annoyingly, the data is lined out as one roll per line. 
    // I want to "transpose" the data, so that I have only on row.
    mat_int* data_tmp = _inData->getDataPtr();

    std::vector<int>* data = new std::vector<int>(data_tmp->size(), 0);
    for (int i = 0; i < data_tmp->size(); i++) {
        (*data)[i] = (*data_tmp)[i][0];
    }

    // Assuming continuous range.
    auto minmax = std::minmax_element(data->begin(), data->end());
    int numObsvStates = *minmax.second;
    numObsvStates += *minmax.first == 0 ? 1 : -*minmax.first;

    // Assuming two hidden states
    int nHiddenstates = 2;

    // Max iteration inital guess 1000
    int max_iter = 1000;

    // Initializing transition matrix A, 
    // emission matrix B, and initial distro vector pi
    // Don't have a good initial guess, assume 
    //  A_{i, j} \approx 1/numHiddenStates
    //  B_{i, j} \approx 1/numObsvStates
    //  pi_i     \approx 1/numHiddenStates

    mat_dbl* initTransMat = new mat_dbl(utils::generateRandomMatrix(nHiddenstates, nHiddenstates, (double)1./nHiddenstates));
    mat_dbl* initEmisMat = new mat_dbl(utils::generateRandomMatrix(nHiddenstates, numObsvStates, (double)1./numObsvStates));
    std::vector<double>* initInitDistro = new std::vector<double>(utils::generateRandomVector(nHiddenstates, (double)1./nHiddenstates));


    // Setting params of HMM
    (*_HMM)->setParams(numObsvStates, data->size(), max_iter, 
        data, nHiddenstates, initTransMat, 
        initEmisMat, initInitDistro);

    return state::GOOD;
}

void Homework4::collectData()
{

    mat_dbl* transMatData = (*_HMM)->transferTransMat();
    mat_dbl* emisMatData = (*_HMM)->transferEmisMat();
    std::vector<double>* initDistroData = (*_HMM)->transferInitialDistro();
    std::vector<double>* logProbData = (*_HMM)->transferLogProbMax();

    std::vector<double> temp = std::vector<double>();
    std::vector<std::vector<double>> outData;

    std::cout<<"Trans Mat"<<std::endl;
    for (int i = 0; i < transMatData->size(); i++) {
        for (int j = 0; j < (*transMatData)[i].size(); j++) {
            std::cout<<(*transMatData)[i][j]<<' ';
            temp.push_back((*transMatData)[i][j]);
        }
        outData.push_back(temp);
        temp.clear();
        std::cout<<std::endl;
    }

    std::cout<<"Emis Mat"<<std::endl;
    for (int i = 0; i < emisMatData->size(); i++) {
        for (int j = 0; j < (*emisMatData)[i].size(); j++) {
            std::cout<<(*emisMatData)[i][j]<<' ';
            temp.push_back((*emisMatData)[i][j]);
        }
        outData.push_back(temp);
        temp.clear();
        std::cout<<std::endl;
    }


    std::cout<<"Initial Distro"<<std::endl;
    for (int i = 0; i < initDistroData->size(); i++) {
        std::cout<<(*initDistroData)[i]<<' ';
        temp.push_back((*initDistroData)[i]);
    }
    outData.push_back(temp);
    temp.clear();
    std::cout<<std::endl;

    for(int i = 0; i < logProbData->size(); i++) {
        if ((*logProbData)[i] == 0) 
        {outData.push_back({(*logProbData)[i-1]});break;}
     
    }
    temp.clear();

    outData.push_back({0});
    // Writing to...
    _outData->setData(outData);
    _outData->writeData(' ', std::ios::app);
    std::cout<<std::endl;

}

void Homework4::doViterbi()
{

    std::vector<int> path = (*_HMM)->viterbi();
    std::cout<<"Path"<<std::endl;

    for (int i = 0; i < path.size(); i++) {
        std::cout<<path[i]<<' ';
    }
}