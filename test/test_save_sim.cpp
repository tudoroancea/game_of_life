#include <iostream>
#include "game_of_life.h"
#include "motifs.h"

using namespace std;

int main() {
    //GameOfLife G(Motif("pentominoR", "local"), 50, 50);
    //G.print();
    //G.life("pentomino1", 10);
    #ifdef TEST_PATH
        cout << "test " << TEST_PATH;
    #endif
    cout << endl;
    #ifdef GLOBAL_PATH
        cout << "global " << GLOBAL_PATH;
    #endif
    cout << endl;
    #ifdef SRC_PATH
        cout << "src " << SRC_PATH;
    #endif
    return 0;
}