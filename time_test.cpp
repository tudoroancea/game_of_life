#include "game_of_life.h"
#include "motifs.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
using namespace std;

int main() {
    ofstream out("time_test.txt");
    vector<int> tailles({10,30,50,100,200,300,500});

    for (auto D : tailles) {
        GameOfLife G(motifs::grenouille,D,D);
    }
    return 0;
}


template<class C>
class Test
{
private:
    /* data */
public:
    Test(/* args */);
    ~Test();
};

Test::Test(/* args */)
{
}

Test::~Test()
{
}


Test<