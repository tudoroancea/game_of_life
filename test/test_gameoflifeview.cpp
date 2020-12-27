#include <iostream>
#include "game_of_life.h"
using namespace std;

int main() {
    GameOfLifeView vue(200,300,200,300);
    //vue.add_motif(Motif(segment({3,1},{5,1})));
    vue.add_cell(3,1);
    vue.add_cell(4,1);
    vue.add_cell(5,1);

    vue.print(cout, false);
    vue.evolve();
    vue.print(cout, false);
    return 0;
}
