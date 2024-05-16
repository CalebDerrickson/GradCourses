#include "utils.hpp"
#include "../LSTM/Gate/Gate.hpp"
#include <memory>

typedef enum output_state {DO_NOT_OUTPUT, OUTPUT} output_state;

// Vanilla LSTM with forget gate. 
class LSTM{
public:
    LSTM();
    ~LSTM();

    void setForgetGateWeights(mat_dbl& weights_) {_forgetGate->Weights = weights_;};
    void setForgetGateBias(vec_dbl& bias_) {_forgetGate->bias = bias_;};

    void setInputGateWeights(mat_dbl& weights_) {_inputGate->Weights = weights_;};
    void setInputGateBias(vec_dbl& bias_) {_inputGate->bias = bias_;};
    
    void setOutputGateWeights(mat_dbl& weights_) {_outputGate->Weights = weights_;};
    void setOutputGateBias(vec_dbl& bias_) {_outputGate->bias = bias_;};

    void setCandGateWeights(mat_dbl& weights_) {_candGate->Weights = weights_;};
    void setCandGateBias(vec_dbl& bias_) {_candGate->bias = bias_;};

    void updateForgetGateWeights(mat_dbl& weights_) {_forgetGate->updateWeights(weights_);};
    void updateForgetGateBias(vec_dbl& bias_) {_forgetGate->updateBias(bias_);};

    void updateInputGateWeights(mat_dbl& weights_) {_inputGate->updateWeights(weights_);};
    void updateInputGateBias(vec_dbl& bias_) {_inputGate->updateBias(bias_);};
    
    void updateOutputGateWeights(mat_dbl& weights_) {_outputGate->updateWeights(weights_);};
    void updateOutputGateBias(vec_dbl& bias_) {_outputGate->updateBias(bias_);};

    void updateCandGateWeights(mat_dbl& weights_) {_candGate->updateWeights(weights_);};
    void updateCandGateBias(vec_dbl& bias_) {_candGate->updateBias(bias_);};


    void resetCellState() {(*_cellState) = std::vector(_cellState->size(), 0.0);};

    // Given an input, push through model and output its result.
    // Corpus is everything needed for backpropagation.
    // aifohs
    void forward(const vec_dbl& input, vec_dbl& output, std::vector<vec_dbl>& corpus);

    void setOutputState(const output_state& newState) {_outputState = newState;};

    std::vector<mat_dbl> getWeights() 
        { std::vector<mat_dbl> mat_arr = {_candGate->Weights, _inputGate->Weights, _forgetGate->Weights, _outputGate->Weights}; return mat_arr;};
    std::vector<vec_dbl> getBiases() 
        { std::vector<vec_dbl> vec_arr = {_candGate->bias, _inputGate->bias, _forgetGate->bias, _outputGate->bias}; return vec_arr;};

    
private:
    std::unique_ptr<Gate> _forgetGate;
    std::unique_ptr<Gate> _outputGate;
    std::unique_ptr<Gate> _inputGate;
    std::unique_ptr<Gate> _candGate;

    std::shared_ptr<vec_dbl> _cellState;
    std::unique_ptr<vec_dbl> _prevOutput;
    output_state _outputState;
    
};