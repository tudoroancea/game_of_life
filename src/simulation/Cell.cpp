//
// Created by Tudor Oancea on 23/03/2021.
//

#include "Cell.hpp"

void Cell::change_state(bool n) {
	if (n != state_) {
		if (n) { for (auto& a : neighbors_) { if (a != nullptr) a->n_live(); }}
		else { for (auto& a : neighbors_) { if (a != nullptr) a->n_die(); }}
		state_ = n;
	}
}

void Cell::add_neigh(Cell* n, unsigned int r) {
	if (neighbors_[r] == nullptr) { neighbors_[r] = n; }
}

