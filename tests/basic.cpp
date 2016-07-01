#include <iostream>

int a = 4;

int main() {
    int *ptr = &a;
    *ptr = 32;

    std::cout << "a is " << a << std::endl;
}
