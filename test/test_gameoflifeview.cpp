#include <iostream>
#include <chrono>
#include <thread>
#include "GameOfLife.hpp"
#include "Motif.hpp"
using namespace std;
using namespace std::chrono_literals;

int main() {
	std::cerr << "hey1" << std::endl;
    GameOfLifeView G(0,20,0,20);
	std::cerr << "hey2" << std::endl;
    G.addMotif(Motif(segment(1,1,1,3)));
	for (size_t i(0); i < 10; ++i) {
		CLEAR()
		G.print();
		this_thread::sleep_for(1s);
	}
    return 0;
}
