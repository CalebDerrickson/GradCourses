#include "HMM.hpp"

#include <iostream>
#include <memory>


HMM::HMM(int numObsvStates, int obsvDataLen, int max_iter,
    std::vector<int>* obsv_data, int nHiddenStates, 
    std::vector<std::vector<double>>* initTransMat, 
    std::vector<std::vector<double>>* initEmisMat, 
    std::vector<double>* initInitDistro) :
    _numObsvStates(numObsvStates), _nHiddenStates(nHiddenStates),
    _max_iter(max_iter), _obsvDataLen(obsvDataLen)
{
    std::cout<<"Allocating Memory..."<<std::endl;

    // Observed data
    _obsv_data = std::make_unique<std::vector<int>>(*obsv_data);    

    // Transition Matrix
    _transMat = std::make_unique<std::vector<std::vector<double>>>(*initTransMat);

    // Emission Matrix
    _emisMat = std::make_unique<std::vector<std::vector<double>>>(*initEmisMat);

    // initial Distro 
    _initialDistro = std::make_unique<std::vector<double>>(*initInitDistro);

    // Betas
    _betas = std::make_unique<std::vector<std::vector<double>>>
        (obsvDataLen, std::vector<double>(nHiddenStates, 0.0));

    // Alphas
    _alphas = std::make_unique<std::vector<std::vector<double>>>
        (obsvDataLen, std::vector<double>(nHiddenStates, 0.0));

    // Gammas
    _gammas = std::make_unique<std::vector<std::vector<double>>>
        (obsvDataLen, std::vector<double>(nHiddenStates, 0.0));

    _digammas = std::make_unique<std::vector<std::vector<std::vector<double>>>>(obsvDataLen,
        std::vector<std::vector<double>>(nHiddenStates, std::vector<double>(nHiddenStates, 0.0)));

    _logProbMax = std::make_unique<std::vector<double>>(max_iter, 0.0);

    _coeffs = std::make_unique<std::vector<double>>(nHiddenStates);

    _internal_state = state::GOOD;
}   

HMM::~HMM()
{


}

state HMM::run()
{
/* Summary of BaumWelch algorithm.
    1. Initialize 
    2. Compute alpha_t(i), beta_t(i), gamma_t(i, j), gamma_t(i)
    3. Re-estimate the model
    4. If P(O | \lambda) increases, go to 2.
*/
    
    for (int iter = 0; iter < _max_iter && _internal_state == state::GOOD;) 
    {
        // Compute Alphas.
        forward();

        // Compute Betas.
        backward();

        // Compute Gammas.
        computeGammas();

        // Restimate
        baumWelch();

        // Compute logProbMax
        computeLogProbMax(iter);

        // Determine iteration
        // will update internal_state
        iterate(iter);
    }
}

void HMM::forward()
{
    // Compute alpha_0(i)
    (*_coeffs)[0] = 0.0;
    for (int i = 0; i < _nHiddenStates; i++) {
        (*_alphas)[0][i] = (*_initialDistro)[i] * (*_emisMat)[i][(*_obsv_data)[0]];
        (*_coeffs)[0] += (*_alphas)[0][i];
    }

    // Scale the alpha_0(i)'s
    (*_coeffs)[0] = 1./(*_coeffs)[0];
    for (int i = 0; i < _nHiddenStates; i++) {
        (*_alphas)[0][i] *= (*_coeffs)[0]; 
    }

    // Compute the alpha_t(i)'s
    for (int t = 1; t < _obsvDataLen; t++) {
        (*_coeffs)[t] = 0.0;
        for (int i = 0; i < _nHiddenStates; i++) {
            (*_alphas)[t][i] = 0.0;
            for (int j = 0; j < _nHiddenStates; j++) {
                (*_alphas)[t][i] += (*_alphas)[t-1][j] * (*_transMat)[j][i]; // OB1 Error?
            }
            (*_alphas)[t][i] = (*_alphas)[t][i] * (*_emisMat)[i][(*_obsv_data)[t]];
            (*_coeffs)[t] += (*_alphas)[t][i];
        }
    
        // Scale the alpha_t(i)'s
        (*_coeffs)[t] = 1./(*_coeffs)[t];
        for (int i = 0; i < _nHiddenStates; i++) {
            (*_alphas)[t][i] *= (*_coeffs)[t];
        }
    
    }

    return;
}

void HMM::backward()
{
    // Compute beta_{T-1}(i)
    for (int i = 0; i < _nHiddenStates; i++) {
        (*_betas)[_obsvDataLen - 1];
    }

    // Compute the betas
    for (int t = _obsvDataLen - 2; t > -1; t--) {
        for (int i = 0; i < _nHiddenStates; i++) {
            (*_betas)[t][i] = 0.0;
            for (int j = 0; j < _nHiddenStates; j++) {
                (*_betas)[t][i] += (*_transMat)[i][j] * 
                    (*_emisMat)[j][(*_obsv_data)[t+1]] 
                    * (*_betas)[t+1][j];
            }
            // Scale beta_t(i)
            (*_betas)[t][i] *= (*_coeffs)[t];
        }
    }

    return;
}

void HMM::computeGammas()
{
    // Compute the bulk of gammas, digammas
    for(int t = 0; t < _obsvDataLen-1; t++) {
        for (int i = 0; i < _nHiddenStates; i++) {
            (*_gammas)[t][i] = 0.0;
            for (int j = 0; j < _nHiddenStates; j++) {
                (*_digammas)[t][i][j] = (*_alphas)[t][i] * (*_transMat)[i][j] * 
                    (*_emisMat)[j][(*_obsv_data)[t+1]] * (*_betas)[t+1][j];
                (*_gammas)[t][i] += (*_digammas)[t][i][j];
            }
        }
    }

    // Special case for T-1
    for (int i = 0; i < _nHiddenStates; i++) {
        (*_gammas)[_obsvDataLen - 1][i] = (*_alphas)[_obsvDataLen - 1][i];
    }
    return;
}

void HMM::computeLogProbMax(int iter)
{
    (*_logProbMax)[iter] = 0;
    for (int i = 0; i < _obsvDataLen; i++) {
        (*_logProbMax)[iter] += std::log((*_coeffs)[i]);
    }
    (*_logProbMax)[iter] *= -1.;
}

void HMM::iterate(int iter)
{
    std::cout<<"Iter : "<<iter<<"\tLogProbMax : "<<(*_logProbMax)[iter]<<std::endl;
    iter++;
    if (iter < _max_iter && (*_logProbMax)[iter-1] < 0) {
        return;
    }
    _internal_state = state::EXIT_GOOD;
    return;
}

state HMM::baumWelch()
{
    // Re-estimate pi's
    for (int i = 0; i < _nHiddenStates; i++) {
        (*_initialDistro)[i] = (*_gammas)[0][i];
    }

    // Re-estimate TransMat
    for (int i = 0; i < _nHiddenStates; i++) {
        double denom = 0;
        for (int t1 = 0; t1 < _obsvDataLen - 1; t1++) {
            denom += (*_gammas)[t1][i];
        }
        for (int j = 0; j < _nHiddenStates; j++) {
            double numer = 0;
            for (int t2 = 0; t2 < _obsvDataLen - 1; t2++) {
                numer += (*_digammas)[t2][i][j];
            }
            (*_alphas)[i][j] = numer / denom;
        }
    }

    // Re-estimate EmisMat
    for (int i = 0; i < _nHiddenStates; i++) {
        double denom = 0;
        for (int t1 = 0; t1 < _obsvDataLen; t1++) {
            denom += (*_gammas)[t1][i];
        }
        for (int j = 0; j < _numObsvStates; j++) {
            double numer = 0;
            for(int t2 = 0; t2 < _obsvDataLen; t2++) {
                if ((*_obsv_data)[t2] == j) 
                    numer += (*_gammas)[t2][i];
            }
            (*_emisMat)[i][j] = numer / denom;
        }
    }
    return;
}
