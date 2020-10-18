#include "game_of_life.h"
#include "motifs.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    GameOfLife G(50,50);
    //liste l1(f2(1,1,8,1));
    //liste l2(f2(8,8,1,8));
    //liste l3(f2(2,3,2,6));
    //liste l4(f2(7,3,7,6));
    //for (auto const& el : l1) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    //for (auto const& el : l2) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    //for (auto const& el : l3) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    //for (auto const& el : l4) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    liste l5(f2(0,0,49,49));
    liste l6(f2(0,0,25,49));
    liste l7(f2(0,0,5,49));
    liste l8(f2(0,0,49,5));
    liste l9(f2(0,0,49,25));
    for (auto const& el : l5) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    for (auto const& el : l6) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    for (auto const& el : l7) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    for (auto const& el : l8) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    for (auto const& el : l9) {cout << el.first << ' ' << el.second << " | "; G.add_cell(el);} cout << endl;
    G.print();
    return 0;
}