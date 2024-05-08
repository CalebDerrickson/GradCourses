#include <iostream>
#include "Homework4.hpp"

int main() {

    Homework4 homework("../data/rolls.txt", "../data/viterbi.txt");
    homework.run();
    homework.doViterbi();
    return 0;
}