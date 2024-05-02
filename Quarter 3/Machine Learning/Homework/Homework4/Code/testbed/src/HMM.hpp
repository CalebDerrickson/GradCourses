#include "utils.hpp"
#include <memory>
#include <vector>

class HMM{
public:
    HMM(int numObsvStates, int obsvDataLen, int max_iter,
        std::vector<int>* obsv_data, int nHiddenStates, 
        std::vector<std::vector<double>>* initTransMat, 
        std::vector<std::vector<double>>* initEmisMat, 
        std::vector<double>* initInitDistro);    
    ~HMM();

    state run();
    void forward();
    void backward();
    void computeGammas();
    void computeLogProbMax(int iter);
    void iterate(int iter);
    state baumWelch();

private:
    // A lot of allocations. 
    // Values to hold internally for efficiency.
    // alphas and betas stored as obsvDataLen x nHiddenStates
    // gammas and digammas stored in a similar way. 
    // For digamma, it is obsvDataLen x nHiddenStates x nHiddenStates
    std::unique_ptr<std::vector<std::vector<std::vector<double>>>> _digammas;
    std::unique_ptr<std::vector<std::vector<double>>> _gammas;
    std::unique_ptr<std::vector<std::vector<double>>> _alphas;
    std::unique_ptr<std::vector<std::vector<double>>> _betas;
    
    // Observed state ptr
    std::unique_ptr<std::vector<int>> _obsv_data;
    int _obsvDataLen;
    int _numObsvStates;

    // Transition matrix b/w states. square 
    std::unique_ptr<std::vector<std::vector<double>>> _transMat;
    int _nHiddenStates;

    // Emission matrix. Size hiddenStates times num observed states 
    std::unique_ptr<std::vector<std::vector<double>>> _emisMat;
    
    // Start probability vector. 
    std::unique_ptr<std::vector<double>> _initialDistro;

    // Maxing the probability by Baum-Welch
    std::unique_ptr<std::vector<double>> _logProbMax;

    int _max_iter;
    state _internal_state;
    std::unique_ptr<std::vector<double>> _coeffs;
};