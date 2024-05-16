#pragma once
#include "LSTM/LSTM.hpp"

#include "FileManager.hpp"
#include "WordEmbedder/WordEmbedder.hpp"
#include "LSTM/Backpropagation/Backpropagation.hpp"

class Homework5 {

public:
    Homework5(const char* embedPath, int numEpocs);
    ~Homework5();

    void init();
    void run();
    void train(int epoch);
    void validate(int epoch);
    void test();
    std::vector<std::vector<vec_dbl>> forwardSentence(const std::vector<mat_dbl>& embedding, std::vector<vec_dbl>& sentenceOutput);
    void backprop(mat_dbl& W, mat_dbl& U, mat_dbl& b, const std::vector<std::vector<vec_dbl>>& corpi, const mat_dbl& actuals, 
        mat_dbl& dW, mat_dbl& dU, mat_dbl& db);
    void GradientDescent(mat_dbl& W, mat_dbl& U, mat_dbl& b, 
        const std::vector<std::vector<vec_dbl>>& corpi, const mat_dbl& actuals, mat_dbl& dW, mat_dbl& dU, mat_dbl& db);


private:

    std::unique_ptr<LSTM> _lstm;
    std::unique_ptr<FileManager<double>> _dblfileManager;
    std::unique_ptr<FileManager<std::string>> _stringFileManager;
    std::unique_ptr<std::vector<std::vector<std::string>>> _trainingData;
    std::unique_ptr<std::vector<std::vector<std::string>>> _validateData;
    std::unique_ptr<std::vector<std::vector<std::string>>> _testingData;
    std::unique_ptr<WordEmbedder> _wordEmbedder;
    std::vector<std::vector<std::vector<double>>> _corpi;
    int _numEpocs;
};