//
// Created by Tudor Oancea on 26/02/2021.
//

#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <future>
#include <list>
#include <numeric>
#include <queue>
#include <deque>

#include "termcolor.hpp"
#include "GameOfLife.hpp"
using namespace std;



int main() {
	CLEAR()
//	std::list<int> l;
//
//	for (size_t i(0); i < 15; ++i) {
//		l.push_front(i);
//		if (l.size() >= 6) l.pop_back();
//		std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " ")); cout << termcolor::blue << termcolor::reset << std::endl;
//	}
	
//	std::array<int,5> tab{0,1,2,3,4};
	std::list<int> tab;
	tab.push_back(0);
	tab.push_back(1);
	tab.push_back(2);
	tab.push_back(3);
	auto it(tab.begin());
	std::cout << termcolor::blue << std::distance(it,tab.begin()) << " : " << *it << termcolor::reset << std::endl;
//	++it;
	std::advance(it, 1);
	std::cout << termcolor::blue << std::distance(it,tab.begin()) << " : " << *it << termcolor::reset << std::endl;
//	++it;
	std::advance(it, 1);
	std::cout << termcolor::blue << std::distance(it,tab.begin()) << " : " << *it << termcolor::reset << std::endl;
//	4 1 0 2 3
	std::cout << boolalpha << (tab.end() == std::prev(tab.begin())) << std::endl;
	return 0;
}