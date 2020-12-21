#include <iostream>
#include "game_of_life.h"
using namespace std;

int main() {
    GameOfLifeView vue(200,300,200,300);
    vue.add_motif(Motif(segment({1,1},{99,99})));
    //vue.add_cell(0,0);
    vue.print(cout, false);
    //vue.evolve();
    return 0;
}
