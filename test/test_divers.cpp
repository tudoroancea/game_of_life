//
// Created by Tudor Oancea on 26/02/2021.
//

#define dx1 0
#define dx2
#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <future>


using namespace std;

void f() {
	auto start = std::chrono::high_resolution_clock::now();
	std::array<int,48> dy{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,-1,-1,-1, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0};
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "f : " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count() << std::endl;
}
void g() {
	auto start = std::chrono::high_resolution_clock::now();
	std::array<int,48> dy{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,-1,-1,-1, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0};
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "g : " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count() << std::endl;
}
enum  E{a,b,c};

int main() {
	return 0;
}