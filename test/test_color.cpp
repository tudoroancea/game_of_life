#include "termcolor.hpp"
#include <iostream>

using namespace std;


int main() {
    cout << termcolor::red << "hey" << termcolor::bold << termcolor::cyan << " and yo" << termcolor::reset << termcolor::red << " to all the hoes" << termcolor::reset << endl;
    std::cout << termcolor::blue << "hey" << std::endl;
    std::cout << termcolor::cyan << "hey" << std::endl;
    return 0;
}