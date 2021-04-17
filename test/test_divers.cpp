//
// Created by Tudor Oancea on 26/02/2021.
//

#include <iostream>
#include <utility>
#include "GameOfLife.hpp"
#include "Motif.hpp"
using namespace std;



int main() {
	Motif M({{0,0},{1,1}});
	for (auto && cell : M) {
		cout << cell << endl;
	}
	for (const auto & cell : M) {
	    cout << cell << endl;
	}
	return 0;
}