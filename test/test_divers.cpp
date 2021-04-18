//
// Created by Tudor Oancea on 26/02/2021.
//

#include <iostream>
#include <utility>
#include "GameOfLife.hpp"
#include "Motif.hpp"
using namespace std;



int main() {
	static GameOfLifeView g(0,5,0,5);
	g.addCell(1,0);
	g.addCell(1,1);
	g.addCell(1,2);
	g.print();
	return 0;
}