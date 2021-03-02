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
using namespace std;


int main() {
//	std::list<int> l (10);
//	std::iota(l.begin(), l.end(),0);
//	auto it(l.begin());
//	++it;
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
//	l.pop_front();
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
//	l.pop_front();
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
//	l.pop_front();
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
//	l.pop_front();
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
//	l.pop_front();
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
//	l.pop_front();
//	std::cout << *it << " | ";
//	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
//	std::cout << std::endl;
	std::list<int> l;
	auto it(l.begin());
	l.push_front(1);
	l.push_front(2);
	l.push_front(3);
	std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl << *it;
	return 0;
}