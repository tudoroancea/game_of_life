//
// Created by Tudor Oancea on 26/02/2021.
//

#include <iostream>
#include <utility>
//#include <array>
//#include <chrono>
//#include <thread>
//#include <future>
//#include <list>
//#include <numeric>
//#include <queue>
//#include <deque>
//
//#include "termcolor.hpp"
#include "GameOfLife.hpp"
#include "Motif.hpp"
using namespace std;

class A;
class B;

class A {
private:
	std::string nom;
	B* b;
public:
	A(std::string&& str = "") : nom(std::move(str)), b(nullptr) {}
	A(std::string&& str, B* b) : nom(std::move(str)), b(b) {}
	B*& getB() {return b;}
	std::string getNom() const {return nom;}
};
class B {
private:
	std::string nom;
	A* a;
public:
	B(std::string&& str = "") : nom(std::move(str)), a(nullptr) {}
	B(std::string&& str, A* a) : nom(std::move(str)), a(a) {}
	A*& getA() {return a;}
	std::string getNom() const {return nom;}
};

int main() {
	A x("x");
	B y("y", &x);
	x.getB() = &y;
	std::cout << x.getNom() << " " << x.getB()->getNom() << std::endl;
	std::cout << y.getNom() << " " << y.getA()->getNom() << std::endl;
	return 0;
}