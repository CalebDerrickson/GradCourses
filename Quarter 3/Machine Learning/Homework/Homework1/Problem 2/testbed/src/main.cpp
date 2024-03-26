#include "Homework2.hpp"
#include <stdlib.h> 

int main() {
    
    const char* filePath = "../toydata.txt";
    int k = 3;

    
    
    // TODO: Remove for loop after 20 times run 
    for (int i = 0; i < 20; i++) {
        _sleep(4000);
        Homework2 homework(filePath, k);
        homework.Run();
    }
    
    return 0;
}