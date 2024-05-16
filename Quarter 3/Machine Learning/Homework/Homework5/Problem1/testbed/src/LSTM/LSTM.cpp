#include "LSTM.hpp"

LSTM::LSTM() {
    _forgetGate = std::make_unique<Gate>();
    _outputGate = std::make_unique<Gate>();
    _inputGate = std::make_unique<Gate>();
    _candGate = std::make_unique<Gate>();
    _candGate->actFunc = utils::act_func::tanh;
    _forgetGate->actFunc = utils::act_func::logistic;
    _outputGate->actFunc = utils::act_func::logistic;
    _inputGate->actFunc = utils::act_func::logistic;


    // Idk size
    _cellState = std::make_unique<vec_dbl>(VECTOR_SIZE, 0.0);
    _prevOutput = std::make_unique<vec_dbl>(VECTOR_SIZE, 0.0);
}


LSTM::~LSTM()
{


}

void LSTM::forward(const vec_dbl& input, vec_dbl& output, std::vector<vec_dbl>& corpus)
{
    // for my sake
    vec_dbl intern_vec(2*VECTOR_SIZE, 0.0f);
    for (int i = 0; i < intern_vec.size(); i++) {
        if (i < _prevOutput->size()) {
            intern_vec[i] = (*_prevOutput)[i];
        }
        if (i >= _prevOutput->size() && (i -_prevOutput->size()) < input.size()) {
            intern_vec[i] = input[i - _prevOutput->size()];
        }
    }

    // Forget Gate

    vec_dbl forgetRes = _forgetGate->forward(intern_vec);

    // Input Gate
    vec_dbl inputRes = _inputGate->forward(intern_vec);

    // Cand Gate
    vec_dbl candRes = _candGate->forward(intern_vec);

    // Update Cell State
    vec_dbl res1 = utils::vecHadamard(forgetRes, *_cellState);
    vec_dbl res2 = utils::vecHadamard(inputRes, candRes);

    _cellState = std::make_unique<vec_dbl>(utils::vecAdd(res1, res2));

    // Output gate
    vec_dbl outputRes = _outputGate->forward(intern_vec);

    // Extract new output
    vec_dbl candtemp = utils::act_func::tanh(*_cellState);

    // Set new output
    vec_dbl new_output = utils::vecHadamard(outputRes, candtemp);
    if (_outputState == output_state::OUTPUT) output = new_output;

    _prevOutput = std::make_unique<vec_dbl>(new_output.begin(), new_output.begin() + 4);

    corpus[0] = candRes;
    corpus[1] = inputRes;
    corpus[2] = forgetRes;
    corpus[3] = outputRes;
    corpus[4] = new_output;
    corpus[5] = *_cellState.get();

}

