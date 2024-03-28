#include "Homework2.hpp"
#include <stdlib.h> 
#include <iostream>




int main() {
    
    const char* filePath = "../data.txt";
    int k = 16;
    // TODO: Remove for loop after 20 times run 
    for (int i = 0; i < 20; i++) {
        _sleep(4000);
        Homework2 homework(filePath, k, "outputDistortion_plusplus_16.txt", flags::KMEANSPP);
        homework.Run();
        std::cout<<"PP16: "<<(i/20.0)*100 <<"%"<<std::endl;
    }


    k = 10;
    // TODO: Remove for loop after 20 times run 
    for (int i = 0; i < 20; i++) {
        _sleep(4000);
        Homework2 homework(filePath, k, "outputDistortion_random_10.txt", flags::RANDOM_CLUSTERS);
        homework.Run();
        std::cout<<"Random10: "<<(i/20.0) *100 <<"%"<<std::endl;
    }

    k = 16;
    // TODO: Remove for loop after 20 times run 
    for (int i = 0; i < 20; i++) {
        _sleep(4000);
        Homework2 homework(filePath, k, "outputDistortion_random_16.txt", flags::RANDOM_CLUSTERS);
        homework.Run();
        std::cout<<"Random10: "<<(i/20.0) *100<<"%"<<std::endl;
    }



    return 0;
}