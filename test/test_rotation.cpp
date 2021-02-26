#include <iostream>
#include "Motif.hpp"
#include "GameOfLife.hpp"

int main() {
    GameOfLifeView G;
    Motif M1(segment(2,0,2,2)), M2(segment(3,9,5,9)), M3(segment(0,3,3,3)), M4(segment(2,0,2,3)), M5(segment(9,3,9,6)), M6(segment(3,9,6,9));
    G.add_motif(M1.rotate(1,{1,2}));
    G.print();
    return 0;
}