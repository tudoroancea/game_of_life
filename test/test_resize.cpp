#include <GameOfLife.hpp>
#include <algorithm>
#include <iostream>
using namespace std;

int main() {
    GameOfLifeView G;
    G.addMotif(Motif(segment(0,0,49,49)));
    G.addMotif(Motif(segment(25,0,0,25)));
    G.print();
    G.zoom(13,13,1.5).print();
    return 0;
}