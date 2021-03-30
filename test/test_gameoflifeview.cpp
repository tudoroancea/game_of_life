#include <iostream>
#include <chrono>
#include <thread>
#include "GameOfLife.hpp"
#include "Motif.hpp"
using namespace std;
using namespace std::chrono_literals;

int main() {
	std::cerr << sizeof(GameOfLife) << std::endl;
	std::cerr << sizeof(GameOfLifeView) << std::endl;
	std::vector<int> v(100000);
	std::cerr << sizeof(std::vector<int>) << std::endl;
	std::cerr << size(v) << std::endl;
	std::cerr << sizeof(std::array<Cell*,8>)+2*sizeof(size_t)+sizeof(bool)+2*sizeof(unsigned int) << std::endl;
	std::cerr << sizeof(Cell) << std::endl;
//	this_thread::sleep_for(3s);
//    static GameOfLifeView G(0,5,0,20);
//	std::cerr << "hey2" << std::endl;
//    G.addMotif(Motif(segment(1,1,1,3)));
//	for (size_t i(0); i < 10; ++i) {
//		CLEAR()
//		G.print();
//		G.evolve();
//		this_thread::sleep_for(1s);
//	}
    return 0;
}
