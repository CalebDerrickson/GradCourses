#include "utils.hpp"

struct Gate{

// Functions
    Gate();
    Gate(mat_dbl& weights, vec_dbl& bias, double (*actFunc)(double));
    ~Gate();


    void resetWeights();
    void resetBias();
    void setActFunc(double (*newActFunc)(double));
    void updateWeights(mat_dbl& weights_);
    void updateBias(vec_dbl& bias_);

    vec_dbl forward(vec_dbl& vec);

// Members
    mat_dbl Weights; 
    vec_dbl bias;
    double (*actFunc)(double val); 
};