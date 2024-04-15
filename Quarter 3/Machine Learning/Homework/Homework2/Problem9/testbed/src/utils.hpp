#include <vector>
#include <cmath>
#include <iostream>




namespace utils{
static double eps = 1e-8;
// Assume the user knows the sizes are the same.
template<typename T1, typename T2>
T1 dot_product(std::vector<T1> &vec1, std::vector<T2>& vec2) 
{
    T1 sum = 0;
    const int size = vec1.size();

    for (int i = 0; i < size; i++) {
        sum += vec1[i] * vec2[i];
    }

    return sum;
}

template <typename T1, typename T2>
std::vector<T1> vector_sum(std::vector<T1> &vec1, std::vector<T2> &vec2)
{
    const int size = vec1.size();
    std::vector<T1> res(size, 0);
 
    for (int i = 0; i < size; i++) {
        res[i] = vec1[i] + (T1)vec2[i];
    }

    return res;
}

template <typename T1, typename T2>
std::vector<T1> vector_diff(std::vector<T1> &vec1, std::vector<T2> &vec2)
{
    const int size = vec1.size();
    std::vector<T1> res(size, 0);
 
    for (int i = 0; i < size; i++) {
        res[i] = vec1[i] - (T1)vec2[i];
    }

    return res;
}


template<typename T1, typename T2>
bool vects_equal(std::vector<T1> &vec1, std::vector<T2> &vec2)
{
    const int size = vec1.size();
    for (int i = 0; i < size; i++) {
        if (std::abs(vec1[i] - (T1)vec2[i]) > eps) return false;
    }
    return true;
}
}