//
// Created by Tudor Oancea on 22/03/2021.
//

#include "MovableGroup.hpp"
#include "CellItem.hpp"
#include "Motif.hpp"
#include "termcolor.hpp"
#include "rapidcsv.h"

#include <QList>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <numeric>

#define N 1000000

using namespace std;
//using namespace

int main() {
//	Timing each method and saving the result in a .csv file =============================================================================

	for (int k(0); k < 100; k++) {
//	    Method 1 ========================================================================
		auto start(chrono::high_resolution_clock::now());
		Motif L;
		for (size_t i(0); i < N; ++i) {
			L.push_back({i, i});
		}
		MovableGroup G1;
		for (auto it = L.begin(); it != L.end(); ++it) {
			auto cell(new CellItem(it->first, it->second));
			G1.append(cell);
		}
		auto stop(chrono::high_resolution_clock::now());
		auto d1(chrono::duration_cast<chrono::microseconds>(stop - start).count());
		std::cerr << termcolor::red << d1 << std::endl;
//	    Method 2 ========================================================================
		start = chrono::high_resolution_clock::now();
		Motif M;
		for (size_t i(0); i < N; ++i) {
			M.push_back({i, i});
		}
		MovableGroup G3(M);
		stop = chrono::high_resolution_clock::now();
		auto d2(chrono::duration_cast<chrono::microseconds>(stop - start).count());
		std::cerr << termcolor::green << d2 << termcolor::reset << std::endl;
		
//	    Method 3 ========================================================================
		start = chrono::high_resolution_clock::now();
		QList<CellItem*> L2;
		for (size_t i(0); i < N; ++i) {
			auto cell(new CellItem(i,i));
			L2.append(cell);
		}
		MovableGroup G2(L2);
		stop = chrono::high_resolution_clock::now();
		auto d3(chrono::duration_cast<chrono::microseconds>(stop - start).count());
		std::cerr << termcolor::yellow << d3 << termcolor::reset << std::endl;

//		Printing the results and cleaning up the allocated memory
		filesystem::current_path(filesystem::path(TEST_PATH));
		ofstream out("test_movablegroup.csv", ios::out | ios::app);
		out << d1 << "," << d2 << "," << d3 << std::endl;
		for (auto& cell : G1.refCells()) {
			delete cell;
		}
		for (auto& cell : G2.refCells()) {
			delete cell;
		}
		for (auto& cell : G3.refCells()) {
			delete cell;
		}
	}

//  Result analysis ==================================================================================================
	filesystem::current_path(filesystem::path(TEST_PATH));
	rapidcsv::Document doc("test_movablegroup.csv", rapidcsv::LabelParams(-1,-1));
	vector<size_t> c1(doc.GetColumn<size_t>(0)), c2(doc.GetColumn<size_t>(1)), c3(doc.GetColumn<size_t>(2));
	long double m1(accumulate(c1.begin(), c1.end(), (long double)0)/(double)c1.size());
	long double m2(accumulate(c2.begin(), c2.end(), (long double)0)/(double)c2.size());
	long double m3(accumulate(c3.begin(), c3.end(), (long double)0)/(double)c3.size());
	std::cout << m1 << std::endl << m2 << endl << m3 << endl;
    return 0;
}
