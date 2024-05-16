#include "Backpropagation.hpp"

Backpropagation::Backpropagation(int train_size, const mat_dbl& actuals_)
{
    corpi = std::vector<std::vector<vec_dbl>>(train_size, mat_dbl(6, vec_dbl(actuals_[0].size(), 0.0)));

}

Backpropagation::~Backpropagation(){}

void Backpropagation::setAll(std::vector<std::vector<vec_dbl>> corpi_,
        const std::vector<vec_dbl>& actuals_, mat_dbl* W_, mat_dbl* U_)
{
    corpi = corpi_;
    actuals = actuals_;
    W = W_;
    U = U_;
}

void Backpropagation::doIt(mat_dbl& dW, mat_dbl& dU, mat_dbl& db)
{
    std::vector<mat_dbl> resW(corpi.size(), mat_dbl(dW.size(), vec_dbl(dW[0].size(), 0.0)));
    std::vector<mat_dbl> resU(corpi.size(), mat_dbl(dU.size(), vec_dbl(dU[0].size(), 0.0)));
    std::vector<mat_dbl> resb(corpi.size(), mat_dbl(4, vec_dbl(corpi[0][0].size(), 0.0)));


    mat_dbl dts(corpi.size(), vec_dbl(actuals.size(), 0.0));
    for (int i = 0; i < dts.size(); i++) {
        dts[i] = utils::loss_func::MSE(actuals[i], corpi[i][4]);
    }

    vec_dbl Dout_t(dts[0].size(), 0.0);

    vec_dbl dstate_t;
    vec_dbl dfuture_state;
    vec_dbl dout_t;
    std::vector<mat_dbl> dgates(corpi.size(), mat_dbl(4, vec_dbl(corpi[0][0].size(), 0.0)));
    
    for (int t = corpi.size()-1; t >= 0; t--) {
        
        // Calculate dout_t
        dout_t = utils::vecAdd(Dout_t, dts[t]);

        

        // Calculate dstate_t
        vec_dbl temp1 = utils::vecHadamard(dout_t, corpi[t][3]);
        
        vec_dbl temp2 = utils::act_func::tanh(corpi[t][5]);
        vec_dbl temp3 = utils::vecHadamard(temp2, temp2);
        vec_dbl temp4(temp3.size(), 0.0);
        for (int i = 0; i < temp4.size(); i++) {
            temp4[i] = 1.0 - temp3[i];
        }
        dstate_t = utils::vecHadamard(temp1, temp4);

        if (t != corpi.size()-1) {
            temp1 = utils::vecHadamard(dfuture_state, corpi[t+1][2]);
            dstate_t = utils::vecAdd(dstate_t, temp1);
        }


        // Calculate delta gates
        // da, di, df, do
        //da
        temp1 = utils::vecHadamard(dstate_t, corpi[t][1]);
        temp2 = utils::vecHadamard(corpi[t][0], corpi[t][0]);
        for (int i = 0; i < temp2.size(); i++) {
            temp2[i] = 1.0 - temp2[i];
        }
        vec_dbl da = utils::vecHadamard(temp1, temp2);

        //di
        temp1 = utils::vecHadamard(dstate_t, corpi[t][0]);
        temp1 = utils::vecHadamard(temp1, corpi[t][1]);
        temp2 = corpi[t][1];
        for (int i = 0; i < temp2.size(); i++) {
            temp2[i] = 1.0 - temp2[i];
        }
        vec_dbl di = utils::vecHadamard(temp1, temp2);

        //df
        vec_dbl df;
        if (t == 0) df = vec_dbl(di.size(), 0.0);
        else {
            temp1 = utils::vecHadamard(dstate_t, corpi[t-1][5]);
            temp1 = utils::vecHadamard(temp1, corpi[t][2]);
            temp2 = corpi[t][2];
            for(int i = 0; i < temp2.size(); i++) {
                temp2[i] = 1.0 - temp2[i];
            }
            df = utils::vecHadamard(temp1, temp2);
        }

        //dog (can't call it do)
        temp1 = utils::act_func::tanh(corpi[t][5]);
        temp1 = utils::vecHadamard(dout_t, temp1);
        temp1 = utils::vecHadamard(temp1, corpi[t][3]);
        temp2 = corpi[t][3];
        for (int i = 0; i < temp2.size(); i++) {
            temp2[i] = 1.0 - temp2[i];
        }
        vec_dbl dog = utils::vecHadamard(temp1, temp2);



        // collect them all in dgates
        dgates[t][0] = da;
        dgates[t][1] = di;
        dgates[t][2] = df;
        dgates[t][3] = dog;

        //TODO: dgates needs to be a 4*VECTOR_SIZE sized vector
        // OR can transform it here
        // could fix it later

        vec_dbl dgates_flat(4*VECTOR_SIZE, 0.0);
        for(int i = 0; i < VECTOR_SIZE; i++) {
            dgates_flat[i + 0*VECTOR_SIZE] = dgates[t][0][i];
            dgates_flat[i + 1*VECTOR_SIZE] = dgates[t][1][i];
            dgates_flat[i + 2*VECTOR_SIZE] = dgates[t][2][i];
            dgates_flat[i + 3*VECTOR_SIZE] = dgates[t][3][i];
        }

        // Also need this for U's
        if (t != corpi.size() - 1) {
            vec_dbl dgates_flat_future(4*VECTOR_SIZE, 0.0);
            for(int i = 0; i < VECTOR_SIZE; i++) {
                dgates_flat_future[i + 0*VECTOR_SIZE] = dgates[t+1][0][i];
                dgates_flat_future[i + 1*VECTOR_SIZE] = dgates[t+1][1][i];
                dgates_flat_future[i + 2*VECTOR_SIZE] = dgates[t+1][2][i];
                dgates_flat_future[i + 3*VECTOR_SIZE] = dgates[t+1][3][i];
            }
            resU[t] = utils::VecVecOuter(dgates_flat_future, corpi[t][4]);
        }

        resW[t] = utils::VecVecOuter(dgates_flat, actuals[t]);
        resb[t] = dgates[t]; 

        // update dfuture_state and Dout
        dfuture_state = dstate_t;
        //Dout = U transpose @ dgates[t] 
        mat_dbl temp = utils::matTranspose(*U);
        Dout_t = utils::MatVecMult(temp, dgates_flat);

    }

    // Collecting terms
    for (int i = 0; i < corpi.size(); i++) {
        dW = utils::MatMatAdd(dW, resW[i]);
        dU = utils::MatMatAdd(dU, resU[i]);
        db = utils::MatMatAdd(db, resb[i]); // ?
    }

}