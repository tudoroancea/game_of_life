#include <iostream>
#include "motifs.h"
#include "game_of_life.h"

int main() {
    GameOfLife G1(11,11), G2(11,11), G3(11,11), G4(11,11), G5(11,11), G6(11,11);
    Motif M1(segment(2,0,2,2)), M2(segment(3,9,5,9)), M3(segment(0,3,3,3)), M4(segment(2,0,2,3)), M5(segment(9,3,9,6)), M6(segment(3,9,6,9));
    //G1.add_motif(M1.rotate(1,{1,2}));
    //G1.print();
    //G2.add_motif(M2.rotate(1,{4,4}));
    //G2.print();
    //G3.add_motif(M3.rotate(-1,{1,2}));
    //G3.print();
    //G4.add_motif(M4.rotate(-1,{0,2}));
    //G4.print();
    //G5.add_motif(M5.rotate(2, {5,5}));
    //G5.print();
    G6.add_motif(M6.rotate(2,{5,4}));
    G6.print();
    return 0;
}