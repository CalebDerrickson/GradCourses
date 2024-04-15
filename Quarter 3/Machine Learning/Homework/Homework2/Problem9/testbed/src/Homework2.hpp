#include "FileManager.hpp"
#include "Perceptron.hpp"
#include <string>


class Homework2 {

public:
    Homework2(int numRuns, double fraction);
    ~Homework2();

    int run();

private:
    int _numRuns;

    std::string outPath;
    std::string inPath;
    Perceptron* _perceptron;
    FileManager<int> _fileManager;

};