#include <iostream>
#include "Motif.hpp"
#include "GameOfLife.hpp"
#include <algorithm>
#include <vector>
#include <iterator>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))


int main()
{
    assert(2+2==4);
    std::cout << "Execution continues past the first assert\n";
    assertm(pow(2,2)==4, "There are five lights");
    std::cout << "Execution continues past the second assert\n";
    assert((2*2==4) && "Yet another way to add assert message");
    return 0;
}