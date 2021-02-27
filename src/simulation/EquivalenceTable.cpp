#include "EquivalenceTable.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
#include <numeric>

SimpleEquivalenceTable::SimpleEquivalenceTable(size_t const& n) : tab(n) {
	std::iota(tab.begin(), tab.end(), 0);
}
void SimpleEquivalenceTable::make_set() {
	tab.push_back(tab.size());
}
size_t SimpleEquivalenceTable::find(size_t x) const {
	while (tab[x] != x) {
		x = tab[x];
	}
	return x;
}
size_t SimpleEquivalenceTable::unionn(size_t x, size_t y) {
	if (x < y) {
		tab[y] = x;
		return x;
	} else {
		tab[x] = y;
		return y;
	}
}

size_t SimpleEquivalenceTable::nbr_classes() const {
	std::vector<size_t> existing;
	//* Version with iterators and fancy functions
	//for (auto const& el: tab) {
	//    if (std::find(existing.begin(), existing.end(), el) == existing.end()) existing.push_back(el);
	//}
	//* Straight-forward version
	size_t j(0);
	for (size_t i(0); i < tab.size() ; ++i) {
		j = 0;
		while (tab[i] != tab[j] && j < i) ++j;
		if (i == j) existing.push_back(i);
	}
	return existing.size();
}
size_t& SimpleEquivalenceTable::operator[](size_t const& n) {return tab[n];}

void SimpleEquivalenceTable::print(std::ostream& out) const {
	if (!tab.empty()) {
		for (size_t i(0); i < tab.size()-1; ++i) out << i << '|';
		out << tab.size()-1 << std::endl;
		for (size_t i(0); i < tab.size() ; ++i) out << "--";
		out << std::endl;
		for (size_t i(0); i < tab.size()-1 ; ++i) out << tab[i] << '|';
		out << tab.back() << std::endl;
	}
}

//================================================================================================================
OptimalEquivalenceTable::OptimalEquivalenceTable(size_t const& n) : tab(n) {
	for (size_t i(0); i < n ; ++i) tab[i] = {i,0};
}
void OptimalEquivalenceTable::make_set() {
	tab.push_back({tab.size(),0});
}
size_t OptimalEquivalenceTable::find_rec(size_t const& x) {
	if (tab[x].parent != x) {
		tab[x].parent = find_rec(tab[x].parent);
		return tab[x].parent;
	} else {
		return x;
	}
}

size_t OptimalEquivalenceTable::find_ite(size_t x) {
	size_t root(x);
	while (tab[root].parent != root) {
		root = tab[root].parent;
	}
	size_t parent(0);
	while (tab[x].parent != root) {
		parent = tab[x].parent;
		tab[x].parent = root;
		x = parent;
	}
	return root;
}
size_t OptimalEquivalenceTable::find_split(size_t x) {
	size_t tpr(0);
	while (tab[x].parent != x) {
		tpr = tab[x].parent;
		x = tab[x].parent;
		tab[x].parent = tab[tpr].parent;
	}
	return x;
}
size_t OptimalEquivalenceTable::find_half(size_t x) {
	while (tab[x].parent != x) {
		tab[x].parent = tab[tab[x].parent].parent;
		x = tab[x].parent;
	}
	return x;
}
size_t OptimalEquivalenceTable::unionn(size_t x, size_t y) {
	x = find_ite(x);
	y = find_ite(y);
	
	if (x != y) {
		if (tab[x].rank < tab[y].rank) {
			std::swap(x, y);
		}
		tab[y].parent = x;
		if (tab[x].rank == tab[y].rank) {
			++tab[x].rank;
		}
	}
	return x;
}

void OptimalEquivalenceTable::print(std::ostream& out) const {
	if (!tab.empty()) {
		for (size_t i(0); i < tab.size()-1; ++i) out << i << '|';
		out << tab.size()-1 << std::endl;
		for (size_t i(0); i < tab.size() ; ++i) out << "--";
		out << std::endl;
		for (size_t i(0); i < tab.size()-1 ; ++i) out << tab[i].parent << '|';
		out << tab.back().parent << std::endl;
	}
}

Node& OptimalEquivalenceTable::operator[](size_t const& n) {
	return tab[n];
}
size_t OptimalEquivalenceTable::nbr_classes() const {
	std::vector<size_t> existing;
	size_t j(0);
	for (size_t i(0); i < tab.size() ; ++i) {
		j = 0;
		while (tab[i].parent != tab[j].parent && j < i) ++j;
		if (i == j) existing.push_back(i);
	}
	return existing.size();
}