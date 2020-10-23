#include "game_of_life.h"
#include "motifs.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <cstdlib>
using namespace std;

int main() {
    GameOfLife G(10,10);
    //liste l5(segment(0,0,49,49));
    //liste l6(segment(0,0,25,49));
    //liste l7(segment(0,0,5,49));
    //liste l8(segment(0,0,49,5));
    //liste l9(segment(0,0,49,25));
    //for (auto const& el : l5) {
    //    //cout << el.first << ' ' << el.second << " | ";
    //    G.add_cell(el);
    //}
    //cout << endl;
    //for (auto const& el : l6) {
    //    //cout << el.first << ' ' << el.second << " | ";
    //    G.add_cell(el);
    //}
    //cout << endl;
    //for (auto const& el : l7) {
    //    //cout << el.first << ' ' << el.second << " | ";
    //    G.add_cell(el);
    //}
    //cout << endl;
    //for (auto const& el : l8) {
    //    //cout << el.first << ' ' << el.second << " | ";
    //    G.add_cell(el);
    //}
    //cout << endl;
    //for (auto const& el : l9) {
    //    //cout << el.first << ' ' << el.second << " | ";
    //    G.add_cell(el);
    //}
    //cout << endl;
    //Motif M({{4,4}});

    //M.push_back({5,8});
    Motif M(segment(4,0,4,4));
    M.append(segment(4,6,4,9));

    G.add_motif(M);
    std::system("clear");
    G.print();
    G.suppr_motif(M);
    M.rotate();
    G.add_motif(M);
    std::this_thread::sleep_for(3s);
    std::system("clear");
    G.print();
    return 0;
}