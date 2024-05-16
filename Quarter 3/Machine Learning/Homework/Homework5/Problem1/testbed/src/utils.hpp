#pragma once
#include <vector>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <omp.h>

typedef std::vector<std::vector<double>> mat_dbl;
typedef std::vector<std::vector<int>> mat_int;
typedef std::vector<int> vec_int;
typedef std::vector<double> vec_dbl;

// Layout of corpus: helpful in forwarding/backprop
constexpr const char* corpusbody = "afiohs";

constexpr int VECTOR_SIZE = 50;
constexpr int NUM_GATES = 4;
constexpr double learning_rate = 1e-2;

namespace utils
{

    inline void printVector(vec_dbl& vec) {
        for(int i = 0; i < vec.size(); i++) {
            std::cout<<vec[i]<<' ';
        }
        std::cout<<std::endl;
    }
    
    inline void printMat(mat_dbl& mat) {
        for(int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[i].size(); j++) {
                std::cout<<mat[i][j]<<' ';
            }
            std::cout<<std::endl;
        }
    }
 

    namespace act_func
    {
        //Linear
        inline double Linear(double val)
        {
            return val;
        }

        //Linear
        inline vec_dbl Linear(vec_dbl& val)
        {
            return val;
        }

        //ReLU
        inline double ReLU(double val) 
        {
            return val > 0 ? val : 0;
        }

        //ReLU
        inline vec_dbl ReLU(vec_dbl& val) 
        {
            vec_dbl res(val.size());
            std::transform(val.begin(), val.end(), res.begin(), [](double x){return ReLU(x);});
            return res;
        }

        //tanh
        inline double tanh(double val)
        {
            return std::tanh(val);
        }

        //tanh
        inline vec_dbl tanh(vec_dbl& val)
        {
            vec_dbl res(val.size());
            std::transform(val.begin(), val.end(), res.begin(), [](double x){ return tanh(x);});
            return res;
        }


        //logistic
        inline double logistic(double val)
        {
            return (double)1/(1.0 + std::exp(-val));
        }

    }

    inline void checkMatrixDimensions(mat_dbl& A) {
        if (A.size() == 0) {
            printf("Matrix has row size zero!\n");
            return;
        }
        
        if (A[0].size() == 0) {
            printf("Matrix has column size zero!\n");
            return;
        }
    }
    
    inline void checkVectorDimensions(vec_dbl& v) {
        if (v.size() == 0) {
            printf("Vector has size zero!\n");
            return;
        }
    }

    inline void checkVectorDimensions(vec_dbl& v1, vec_dbl& v2) {
        checkVectorDimensions(v1);
        checkVectorDimensions(v2);
        if (v1.size() != v2.size()) {
            printf("utils::checkVectorDimensions: vectors not same sizes. v1: %d. v2: %d\n", v1.size(), v2.size());
        }
    }

    inline mat_dbl VecVecOuter(vec_dbl& vec1, vec_dbl& vec2) 
    {
        checkVectorDimensions(vec1);
        checkVectorDimensions(vec2);
        mat_dbl res(vec1.size(), vec_dbl(vec2.size(), 0.0));

        for (int i = 0; i < vec1.size(); i++) {
            for (int j = 0; j < vec2.size(); j++) {
                res[i][j] = vec1[i] * vec2[j];
            }
        }
        return res;
    }

    inline vec_dbl MatVecMult(mat_dbl& A, vec_dbl& x) {
        if (A[0].size() != x.size()) {
            printf("Utils::MatVecMult: A and x not compatible. A size (%d, %d). x size %d\n",
                A.size(), A[0].size(), x.size());
            return x;
        }

        vec_dbl res(A.size(), 0.0);

        #pragma omp parallel for
        for (int i = 0; i < A.size(); ++i) {
            double sum = 0.0;
            for (int j = 0; j < A[i].size(); ++j) {
                sum += A[i][j] * x[j];
            }
            #pragma omp critical
            res[i] = sum;
        }

        return res;
    }

    // inline mat_dbl MatMatMult(mat_dbl& A, mat_dbl& B) {
    //     checkMatrixDimensions(A);
    //     checkMatrixDimensions(B);
    //     if (A[0].size() != B.size()) {
    //         printf("MatMatMult Error: Matrices are not compatible sizes. A: (%d, %d), B: (%d, %d)\n",
    //         A.size(), A[0].size(), B.size(), B[0].size());
    //         return A;
    //     }
// 
    //     int m = A.size();
    //     int n = B[0].size();
    //     int p = B.size();
// 
    //     mat_dbl res(m, vec_dbl(n, 0.0));
// 
    //     for (int i = 0; i < m; ++i) {
    //         for (int j = 0; j < n; ++j) {
    //             for (int k = 0; k < p; ++k) {
    //                 res[i][j] += A[i][k] * B[k][j];
    //             }
    //         }
    //     }
    //     return res;
    // }

    inline mat_dbl MatMatMult(mat_dbl& A, mat_dbl& B) {
        checkMatrixDimensions(A);
        checkMatrixDimensions(B);
        if (A[0].size() != B.size()) {
            printf("MatMatMult Error: Matrices are not compatible sizes. A: (%d, %d), B: (%d, %d)\n",
            A.size(), A[0].size(), B.size(), B[0].size());
            return A;
        }

        int m = A.size();
        int n = B[0].size();
        int p = B.size();

        mat_dbl res(m, vec_dbl(n, 0.0));

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < p; ++k) {
                    #pragma omp atomic
                    res[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return res;
    }


    inline mat_dbl matTranspose(mat_dbl& mat) 
    {
        checkMatrixDimensions(mat);
        mat_dbl res(mat[0].size(), vec_dbl(mat.size(), 0.0));
        for (int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[i].size(); j++) {
                res[j][i] = mat[i][j];
            }
        }
        return res;
    }

    inline vec_dbl vecAdd(vec_dbl& vec1, vec_dbl& vec2) {
        checkVectorDimensions(vec1, vec2);
        vec_dbl res(vec1.size(), 0.0);
        for (int i = 0; i < vec1.size(); i++) {
            res[i] = vec1[i] + vec2[i];
        }

        return res;
    }

    inline vec_dbl vecHadamard(vec_dbl& vec1, vec_dbl& vec2) {
        checkVectorDimensions(vec1, vec2);

        vec_dbl res(vec1.size(), 0.0);
        for (int i = 0; i < vec1.size(); i++) {
            res[i] = vec1[i] * vec2[i];
        }

        return res;
    }

    inline vec_dbl vecScale(vec_dbl& vec, double scale) {
        vec_dbl res(vec.size(), 0.0);
        for (int i = 0; i < vec.size(); i++) {
            res[i] = scale * vec[i];
        }

        return res;
    }

    inline mat_dbl matScale(mat_dbl& mat, const double& num) {
        checkMatrixDimensions(mat);
        mat_dbl res(mat);
        for (int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[i].size(); j++) {
                res[i][j] = num * res[i][j];
            }
        }
        return res;
    }

    inline mat_dbl matVecOuterProduct(mat_dbl& mat, vec_dbl& vec) 
    {
        checkVectorDimensions(vec);
        checkMatrixDimensions(mat);
        if (vec.size() != mat[0].size()) {
            printf("OuterProduct error: matrix row size and vector size are not equal! matrix: (%d, %d), vec: %d\n",
            mat.size(), mat[0].size(), vec.size());
            return mat;
        }

        mat_dbl res(mat.size(), vec_dbl(mat[0].size(), 0.0));
        for (int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[i].size(); j++) {
                res[i][j] = mat[i][j] * vec[j];
            }
        }
        return res;


    }

    inline mat_dbl MatMatAdd(mat_dbl& A, mat_dbl& B) {
        checkMatrixDimensions(A);
        checkMatrixDimensions(B);
        if (A.size() != B.size()  || A[0].size() != B[0].size()) {
            printf("MatMatAdd: Dimensions not work. A: (%d, %d), B: (%d, %d)\n",
            A.size(), A[0].size(), B.size(), B[0].size());
            return A;
        }

        mat_dbl res = A;
        for (int i = 0; i < A.size(); i++) {
            for (int j = 0; j < A[i].size(); j++) {
                res[i][j] += B[i][j];
            }
        }
        return res;
    }

    namespace loss_func 
    {
        inline double MSE(const vec_dbl& actual, const vec_dbl& pred) {
            double res = 0.0;
            for (int i = 0; i < actual.size(); i++) {
                res += (actual[i] - pred[i]) * (actual[i] - pred[i]);
            }
            res /= 2.;

            return res;
        }

        inline vec_dbl MSE(vec_dbl& actual, vec_dbl& pred) {
            vec_dbl res(actual.size());
            for (int i = 0; i < actual.size(); i++) {
                res[i] = (actual[i] - pred[i]) * (actual[i] - pred[i]);
                res[i] /= 2.;
            }
            return res;
        }

    }
}