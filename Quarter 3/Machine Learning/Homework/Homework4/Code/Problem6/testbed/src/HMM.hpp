#include "utils.hpp"
#include <memory>
#include <vector>

class HMM{
public:

    HMM();

    HMM(int numObsvStates, int obsvDataLen, int max_iter,
        std::vector<int>* obsv_data, int nHiddenStates, 
        mat_dbl* initTransMat, 
        mat_dbl* initEmisMat, 
        std::vector<double>* initInitDistro);    
    ~HMM();

    void run();
    void forward();
    void backward();
    void computeGammas();
    void computeLogProbMax(int& iter);
    void iterate(int& iter);
    void baumWelch();
    std::vector<int> viterbi();

    void setParams(int numObsvStates, int obsvDataLen, int max_iter,
        std::vector<int>* obsv_data, int nHiddenStates, 
        mat_dbl* initTransMat, 
        mat_dbl* initEmisMat, 
        std::vector<double>* initInitDistro);

    // Getters / Transferers
    std::vector<double>* transferLogProbMax() 
        {return _logProbMax.get();}

    mat_dbl* transferTransMat() 
        {return (*_transMat);}
    
    mat_dbl* transferEmisMat() 
        {return (*_emisMat);}

    std::vector<double>* transferInitialDistro()
        {return (*_initialDistro);}

private:
    // A lot of allocations. 
    // Values to hold internally for efficiency.
    // alphas and betas stored as obsvDataLen x nHiddenStates
    // gammas and digammas stored in a similar way. 
    // For digamma, it is obsvDataLen x nHiddenStates x nHiddenStates
    std::unique_ptr<std::vector<mat_dbl>> _digammas;
    std::unique_ptr<mat_dbl> _gammas;
    std::unique_ptr<mat_dbl> _alphas;
    std::unique_ptr<mat_dbl> _betas;
    
    // Observed state ptr
    std::unique_ptr<std::vector<int>*> _obsv_data;
    int _obsvDataLen;
    int _numObsvStates;

    // Transition matrix b/w states. square 
    std::unique_ptr<mat_dbl*> _transMat;
    int _nHiddenStates;

    // Emission matrix. Size hiddenStates times num observed states 
    std::unique_ptr<mat_dbl*> _emisMat;
    
    // Start probability vector. 
    std::unique_ptr<std::vector<double>*> _initialDistro;

    // Maxing the probability by Baum-Welch
    std::unique_ptr<std::vector<double>> _logProbMax;

    int _max_iter;
    state _internal_state;
    std::unique_ptr<std::vector<double>> _coeffs;
};