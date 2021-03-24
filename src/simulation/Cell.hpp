//
// Created by Tudor Oancea on 23/03/2021.
//

#ifndef GAME_OF_LIFE_CELL_HPP
#define GAME_OF_LIFE_CELL_HPP

#include "Motif.hpp"

#include <array>
#include <iostream>


class Cell;

typedef std::array<Cell*,8> neighborhood;

class Cell {
public:
	explicit Cell(neighborhood n = {nullptr}, Coord c = {0,0}, bool s=false, unsigned int l=0, unsigned int g=0)
			: neighbors_(n), coords_(c), state_(s), living_(l), gen_seen_(g) {}
	Cell(Cell const& c) = delete;
	void add_neigh(Cell* n, unsigned int r=0);
	void change_state(bool n);
	[[nodiscard]] bool get_state() const {return state_;}
	[[nodiscard]] unsigned int living_neighbors() const {return living_;}
	[[nodiscard]] Coord get_coords() const {return coords_;}
	void set_coords(Coord c) {coords_ = c;}
	[[nodiscard]] unsigned int gen_() const {return gen_seen_;}
	void set_gen_(unsigned int& g) {gen_seen_ = g;}
	void n_die() {living_--;}
	void n_live() {living_++;}

private:
	neighborhood neighbors_;
	Coord coords_;
	bool state_;
	unsigned int living_;
	unsigned int gen_seen_;
};


#endif //GAME_OF_LIFE_CELL_HPP
