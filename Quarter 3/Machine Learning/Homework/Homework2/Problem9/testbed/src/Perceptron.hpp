#include <vector>
#include <memory>

// Single class Perceptron

class Perceptron {

public:
    Perceptron();

    Perceptron(
        std::vector<std::vector<int>>* testData, 
        std::vector<std::vector<int>>* trainData, 
        std::vector<int>* predictionLabels
    );

    ~Perceptron();

    int run(int numRuns);
    void clearWeights();

    std::vector<double> getWeights() {return _weights;}
    std::vector<double> getAccuracies() {return _accuracy;}
    std::vector<int> getPredictions() {return _predictions;}

    // Used for individual predictions
    void predict(int &predict, std::vector<int>& predictVect);

private:
    // Train will run over all the training data, after each iteration will test
    int train(int numRun);

    // tests ONLY once, at the end of all runs.
    int test();


private:

    std::vector<double> _weights;
    std::vector<std::vector<int>>* _testData;
    std::vector<std::vector<int>>* _trainData; 
    std::vector<int>* _predictionLabels; 
    
    std::vector<double> _accuracy;

    // The predictions of the testing data
    std::vector<int> _predictions;


};