#include "GameOfLife.hpp"
#include "Motif.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
using namespace std;

int main() {
    ofstream out("time_test.txt");
    // vector<int> tailles({10,30,50,100,200,300,500});

    /* std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
    for (auto D : tailles) {
        start = std::chrono::high_resolution_clock::now();
        GameOfLife G(grenouille,D,D);
        stop = std::chrono::high_resolution_clock::now();
        cout << "Temps d'instanciation pour grille de " << D << " : " << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << endl;

        start = std::chrono::high_resolution_clock::now();
        G.evolve();
        stop = std::chrono::high_resolution_clock::now();
        cout << "Temps d'evolution pour grille de " << D << " : " << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << endl;
    } */

    // vector<unsigned int> v;
    double instanciation(0.0), evolution(0.0);
    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
    for (size_t i(0); i < 200 ; ++i) {
        start = std::chrono::high_resolution_clock::now();
        GameOfLife G;
        stop = std::chrono::high_resolution_clock::now();
        instanciation += std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count();
        if(i%10 == 0) cout << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << endl;
        start = std::chrono::high_resolution_clock::now();
        G.evolve();
        stop = std::chrono::high_resolution_clock::now();
        evolution += std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count();
    }
    cout << instanciation/200 << endl << evolution/200 << endl;
    return 0;
}
