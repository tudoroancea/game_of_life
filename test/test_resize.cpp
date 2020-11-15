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
    liste v(G.get_viv());
    for (auto const& el : v) cout << "(" << el << ")\n";
    G.resize(40,40);
    G.print();
    v = G.get_viv();
    for (auto const& el : v) cout << "(" << el << ")\n";
    return 0;
}