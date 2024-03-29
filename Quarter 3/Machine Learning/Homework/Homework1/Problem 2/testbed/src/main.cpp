#include "Homework2.hpp"
#include <stdlib.h> 
#include <iostream>




int main() {
    
    const char* filePath = "../Example.txt";
    int k = 2;
    // TODO: Remove for loop after 20 times run 
    for (int i = 0; i < 20; i++) {
        _sleep(2000);
        Homework2 homework(filePath, k, "KmeansExample.txt", flags::KMEANSPP);
        homework.Run();
        std::cout<<"Kmeans++: "<<((i+1)/20.0)*100 <<"%"<<std::endl;
    }

    for (int i = 0; i < 20; i++) {
        _sleep(2000);
        Homework2 homework(filePath, k, "randomExample.txt", flags::RANDOM_CLUSTERS);
        homework.Run();
        std::cout<<"Random: "<<((i+1)/20.0)*100 <<"%"<<std::endl;
    }


    return 0;
}