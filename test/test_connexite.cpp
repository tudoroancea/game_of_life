#include "GameOfLife.hpp"
#include "EquivalenceTable.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>
#include <iterator>
using namespace std::chrono;

int main() {
    for (auto const& direction : {1,2,3}) {
        std::cout << direction << ' ';
    }
    std::cout << std::endl;

    //    std::cout << el.first << ' ' << el.second << '|';
    //}
    std::cout << std::endl;
    return 0;
}
