#include "utils.hpp"
//https://medium.com/@aidangomez/let-s-do-this-f9b699de31d9
struct Backpropagation
{
//Functions
    Backpropagation(int train_size, const mat_dbl& actuals_);
    ~Backpropagation();

    // db might be wrong. shouldn't it be vec_dbl? 
    void doIt(mat_dbl& dW, mat_dbl& dU, mat_dbl& db);
    void setAll(std::vector<std::vector<vec_dbl>> corpi_,
        const std::vector<vec_dbl>& actuals_, mat_dbl* W_, mat_dbl* U_);

// Members

    // all values from training. aifohs for all results in training. 
    std::vector<std::vector<vec_dbl>> corpi;

    mat_dbl actuals;
    mat_dbl* W;
    mat_dbl* U;
};
