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
#include <list>
#include <numeric>

#include "termcolor.hpp"

using namespace std;


int main() {
	std::list<int> l;
	
	l.push_front(2);
	l.push_front(1);
	l.push_front(0);
	auto it(l.begin());
	++it;
	std::cout << *it << std::endl;
	l.erase(l.begin(), it);
	std::cout << *it << std::endl;
	if (it == l.begin()) std::cout << "\nhey\n";
	return 0;
}