#ifndef EQ_TABLE_H
#define EQ_TABLE_H

#include <array>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

class SimpleEquivalenceTable {
private :
	std::vector<size_t> tab;
public :
	SimpleEquivalenceTable() = default;
	explicit SimpleEquivalenceTable(size_t const& n);
	SimpleEquivalenceTable(SimpleEquivalenceTable const&) = default;
	SimpleEquivalenceTable(SimpleEquivalenceTable &&) = default;
	
	/**
	 * @brief add a new equivalence class to the table
	 *
	 */
	void make_set();
	/**
	 * @brief Iterative version without parent update along the way
	 */
	size_t find(size_t x) const;
	/**
	 * @brief
	 *
	 * @param x
	 * @param y
	 * @return size_t
	 */
	size_t unionn(size_t x, size_t y);
	
	/**
	 * @return number of different labels in the equivalence table (if )
	 */
	size_t nbr_classes() const;
	
	void print(std::ostream& out = std::cout) const;
	
	size_t& operator[](size_t const& n);
	
};
struct Node {
	size_t parent;
	size_t rank;
};
class OptimalEquivalenceTable {
private :
	std::vector<Node> tab;
public :
	OptimalEquivalenceTable() = default;
	explicit OptimalEquivalenceTable(size_t const& n);
	OptimalEquivalenceTable(OptimalEquivalenceTable const&) = default;
	OptimalEquivalenceTable(OptimalEquivalenceTable &&) = default;
	
	/**
	 * @brief add a new equivalence class to the table
	 *
	 */
	void make_set();
	/**
	 * @brief Recursive version
	 *
	 * @param x
	 * @return size_t
	 */
	size_t find_rec(size_t const& x);
	/**
	 * @brief Iterative version with constant memory
	 *
	 * @param x
	 * @return size_t
	 */
	size_t find_ite(size_t x);
	/**
	 * @brief Path-splitting version (without full parent pointer update, ie not along the whole path)
	 */
	size_t find_split(size_t x);
	/**
	 * @brief Path-halving version (without full parent pointer update, ie not along the whole path)
	 *
	 * @param x
	 * @return size_t
	 */
	size_t find_half(size_t x);
	/**
	 * @brief
	 *
	 * @param x
	 * @param y
	 * @return root of the equivalence class after fusion
	 */
	size_t unionn(size_t x, size_t y);
	
	size_t nbr_classes() const;
	
	void print(std::ostream& out = std::cout) const;
	
	Node& operator[](size_t const& n);
};

#endif  // EQ_TABLE_H