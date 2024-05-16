#include "Homework5.hpp"

int main() {
    {
        Homework5 homework("../data/bobsue.voc.txt", 5);
        homework.init();
        homework.run();
    }
}