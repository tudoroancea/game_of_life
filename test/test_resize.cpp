#include <game_of_life.h>
#include <algorithm>
#include <iostream>
using namespace std;

int main() {
    GameOfLife G(50,50);
    G.add_cell(0,0);
    G.add_cell(9,9);
    G.add_cell(10,10);
    G.add_cell(19,19);
    G.add_cell(20,20);
    G.add_cell(29,29);
    G.add_cell(30,30);
    G.add_cell(39,39);
    G.add_cell(40,40);
    G.add_cell(49,49);
    G.print();
    //std::array<std::array<bool,500>,500> tab(G.get_champ());
    //for (auto const& ligne : tab) {
    //    for (auto const& el : ligne) {
    //        if (el != 0)
    //    }
    //}

    //liste v(G.get_viv_2());
    //for (auto const& el : v) cout << el << endl;

    G.resize(40,40);
    G.print();
    G.resize(50, 50);
    G.print();
    return 0;
}