#include "EquivalenceTable.hpp"
#include "termcolor.hpp"

#include <chrono>
#include <iostream>

using namespace std;

#define N 100000000
int main() {
    SimpleEquivalenceTable T1(N);
    for (size_t i(1); i < N ; ++i) T1[i]=i-1;
    SimpleEquivalenceTable T2(T1);
    auto start1(std::chrono::high_resolution_clock::now());
    size_t root1(T1.find(N-1));
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto start2(std::chrono::high_resolution_clock::now());
    size_t root2(T2.find(N-1));
    auto stop2 = std::chrono::high_resolution_clock::now();
    std::cout << root1 << " : " << std::chrono::duration_cast<std::chrono::microseconds>(stop1-start1).count() << std::endl << root2 << " : " << std::chrono::duration_cast<std::chrono::microseconds>(stop2-start2).count() << std::endl;

    //OptimalEquivalenceTable T1(6);
    //T1[2].parent = 1;
    //T1[4].parent = 2;
    //T1[5].parent = 4;
    //OptimalEquivalenceTable T2(T1), T3(T1), T4(T1);
    //std::cerr << termcolor::yellow << "T1" << termcolor::reset << std::endl;
    //T1.find1(5);
    //T1.print();
    //std::cerr << termcolor::yellow << "T2" << termcolor::reset << std::endl;
    //T2.find2(5);
    //T2.print();
    //std::cerr << termcolor::yellow << "T3" << termcolor::reset << std::endl;
    //T3.find3(5);
    //T3.print();
    //std::cerr << termcolor::yellow << "T4" << termcolor::reset << std::endl;
    //T4.find4(5);
    //T4.print();

    return 0;
}