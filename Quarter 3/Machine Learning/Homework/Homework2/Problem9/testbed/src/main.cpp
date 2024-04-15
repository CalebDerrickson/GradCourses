#include "Homework2.hpp"

int main() {
int num = 20;
for (int i = 1; i <= num; i++) {
    Homework2 homework(10, (i/(float)num));
    homework.run();
}

}