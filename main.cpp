#include <iostream>
#include <cmath>
#include <array>
#include <cstdlib>
#include <vector>
#include <thread>
using namespace std;

typedef vector<pair<size_t,size_t>> vec;

class Motif {
private :
    vec liste;
    size_t min_ligne() const {
        size_t min(liste.front().first);
        for (auto el : liste) {
            if (el.first < min) min = el.first;
            if (min == 0) break;
        }
        return min;
    }
    size_t min_colonne() const {
        size_t min(liste.front().second);
        for (auto el : liste) {
            if (el.second < min) min = el.second;
            if (min == 0) break;
        }
        return min;
    }
    size_t max_ligne() const {
        size_t max(liste.front().first);
        for (auto el : liste) {
            if (el.first > max) max = el.first;
        }
        return max;
    }
    size_t max_colonne() const {
        size_t max(liste.front().second);
        for (auto el : liste) {
            if (el.second > max) max = el.second;
        }
        return max;
    }
public :
    Motif() = default;
    Motif(initializer_list<pair<size_t,size_t>> L) : liste(L) {}

    void operator+=(Motif const& rhs) {
        for (auto const& el : rhs.liste) liste.push_back(el);
    }

    void operator+=(pair<size_t,size_t> const& p) {
        for (auto& el : liste) {
            el.first += p.first;
            el.second += p.second;
        }
    }
    void operator-=(pair<size_t,size_t> const& p) {
        size_t l(min_ligne()), c(min_colonne());
        if (p.first>l) {
            for (auto& el : liste) el.first -= l;
        } else {
            for (auto& el : liste) el.first -= p.first;
        }
        if (p.second>c) {
            for (auto& el : liste) el.second -= c;
        } else {
            for (auto& el : liste) el.second -= p.second;
        }
    }
    vec::iterator begin() {return liste.begin();}
    vec::iterator end() {return liste.end();}
    vec::const_iterator cbegin() const {return liste.cbegin();}
    vec::const_iterator cend() const {return liste.cend();}
};

template<unsigned int L, unsigned int C>
class JeuDeLaVie {
private :
	array<array<bool,C>,L> champ;
	std::chrono::duration<double, std::milli> tick_period;

	bool access(size_t i, size_t j) {
		if (i>=L or j>=C) return false;
		else return champ[i][j];
	}
	bool next_state(size_t i, size_t j) {
	int S(0);
	if (access(i - 1, j - 1)) ++S;
	if (access(i - 1, j)) ++S;
	if (access(i - 1, j + 1)) ++S;
	if (access(i, j - 1)) ++S;
	if (access(i, j + 1)) ++S;
	if (access(i + 1, j - 1)) ++S;
	if (access(i + 1, j)) ++S;
	if (access(i + 1, j + 1)) ++S;
	return (S==3) or (access(i, j) and S == 2);
}
public :
	JeuDeLaVie(Motif const& a_marquer, std::chrono::duration<double, std::milli> const& t = 100.0ms) : tick_period(t) {
		for (size_t i(0); i < L ; ++i) {
			for (size_t j(0); j < C ; ++j) champ[i][j] = false;
		}
		for (vec::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) champ[it->first][it->second] = true;
	}
	void print(std::ostream& out = cout) const {
        out << "Debut" << endl;
        for (size_t i(0); i < L; ++i) {
			for (size_t j(0); j < C; ++j) {
				if (champ[i][j]) out << 1;
				else out << ' ';
			}
			out << endl;
		}
        out << " Fin" << endl;
	}
	void evolve() {
		array<array<bool,C>,L> res;
		for (size_t i(0); i < L ; ++i) {
			for (size_t j = 0; j < C ; ++j) {
				res[i][j]=next_state(i,j);
			}
		}
		champ=res;
	}
	void life(chrono::milliseconds t, std::ostream& out = cout) {
		long unsigned int repets(std::floor(t.count()/tick_period.count()));
		out << repets << endl;
		for (long unsigned int i(0); i < repets ; ++i) {
			print(out);
			evolve();
			this_thread::sleep_for(tick_period);
			system("clear");
		}
	}
};


int main() {
	Motif grenouille({{1,1},{1,2},{1,3},{2,0},{2,1},{2,2}});
	Motif planeur({{0,1},{1,2},{2,0},{2,1},{2,2}});
    Motif gland({{0,1},{1,3},{2,0},{2,1},{2,4},{2,5},{2,6},{2,7}});
    gland+={12,12};
    Motif lievres({{1,0},{1,2},{2,1},{3,1},{0,4},{0,6},{1,5},{2,5},{3,7}});
    lievres+={15,12};

    Motif canon({{5,0},{5,1},{6,0},{6,1},
    {5,10},{6,10},{7,10},{4,11},{3,12},{3,13},{4,15},{5,16},{6,16},{7,16},{6,17},{6,14},{8,15},{9,13},{9,12},{8,11}
    });
    Motif ligne({{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,10}});
    ligne+={10,15};
    JeuDeLaVie<30,30> G(ligne);
    G.print();
    this_thread::sleep_for(1s);
    G.life(10000ms);
    Motif baton({{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{1,0},{1,1},{1,2},{1,3},{1,4},{1,5}});

	return 0;
}
