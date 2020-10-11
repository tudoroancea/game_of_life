#include <iostream>
#include <cmath>
#include <array>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;


typedef vector<pair<size_t,size_t>> vec;

namespace motifs {
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
        // Constructeurs & Destructeurs
        Motif() = default;
        Motif(Motif const&) = default;
        Motif(initializer_list<pair<size_t,size_t>> L) : liste(L) {}

        // Méthodes de modification
        void push_back(pair<size_t,size_t> const& p) {liste.push_back(p);}
        void rotate()
        // 90° en sens horaire (anti trigo)
        {
            size_t max(max_ligne()), tpr(0);
            for (auto& el : liste) {
                tpr = max-el.first;
                el.first = el.second;
                el.second = tpr;
            }
        }
        void rotate2() {
            if (min_ligne()-max_colonne()>=0) {
                size_t ibis(0),jbis(0),l(min_ligne()),c(min_colonne());
                for (auto& el : liste) {
                    jbis = c+el.first;
                    ibis = l-el.second;
                    el.first = ibis;
                    el.second = jbis;

                }
            }
        }
        Motif& operator+=(Motif const& rhs) {
            for (auto const& el : rhs.liste) liste.push_back(el);
            return *this;
        }
        Motif& operator+=(pair<size_t,size_t> const& p) {
            for (auto& el : liste) {
                el.first += p.first;
                el.second += p.second;
            }
            return *this;
        }
        Motif& operator-=(pair<size_t,size_t> const& p) {
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
            return *this;
        }
        Motif& recalibrate() {(*this)-={min_ligne(), min_colonne()}; return *this;}

        // Getters
        vec::iterator begin() {return liste.begin();}
        vec::iterator end() {return liste.end();}
        vec::const_iterator cbegin() const {return liste.cbegin();}
        vec::const_iterator cend() const {return liste.cend();}
    };
    Motif operator+(Motif lhs, Motif const& rhs) {return lhs+=rhs;}
    Motif operator+(Motif lhs, pair<size_t,size_t> const& rhs) {return lhs+=rhs;}

    // Modèles déjà créés
    const Motif grenouille({{1,1},{1,2},{1,3},{2,0},{2,1},{2,2}});
	const Motif planeur({{0,1},{1,2},{2,0},{2,1},{2,2}});
    const Motif gland({{0,1},{1,3},{2,0},{2,1},{2,4},{2,5},{2,6},{2,7}});
    const Motif canon({{5,0},{5,1},{6,0},{6,1},
    {5,10},{6,10},{7,10},{4,11},{3,12},{3,13},{4,15},{5,16},{6,16},{7,16},{6,17},{6,14},{8,15},{9,13},{9,12},{8,11}
    });
    const Motif lievres({{1,0},{1,2},{2,1},{3,1},{0,4},{0,6},{1,5},{2,5},{3,7}});

    // Construction de motifs géométriques
    Motif rectangle(size_t lignes, size_t colonnes) {
        Motif res;
        for (size_t i(0); i < lignes ; ++i) {
            for (size_t j(0); j < colonnes ; ++j) res.push_back({i,j});
        }
        return res;
    }
    Motif ligneh(size_t longueur) {
        Motif res;
        for (size_t i(0); i < longueur ; ++i) res.push_back({i,0});
        return res;
    }
    Motif lignev(size_t longueur) {
        Motif res;
        for (size_t j(0); j < longueur ; ++j) res.push_back({0,j});
        return res;
    }
} // namespace motifs

using namespace motifs;

template<unsigned int L, unsigned int C>
class GameOfLife {
private :
	array<array<bool,C>,L> champ;
	std::chrono::duration<double, std::milli> tick_period;
    unsigned int nbr_gen;

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
	GameOfLife(motifs::Motif const& a_marquer, std::chrono::duration<double, std::milli> const& t = 100.0ms) : tick_period(t), nbr_gen(0) {
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
        out << " Fin | Generation num " << nbr_gen << endl;
	}
	void evolve() { // todo : optimiser les zones vides
        array<array<bool,C>,L> res;
        auto start(std::chrono::high_resolution_clock::now());
		for (size_t i(0); i < L ; ++i) {
			for (size_t j = 0; j < C ; ++j) {
				res[i][j]=next_state(i,j);
			}
		}
		champ=res;
        auto stop(std::chrono::high_resolution_clock::now());
        ++nbr_gen;
	}
	void life(chrono::milliseconds t, std::ostream& out = cout) {
		long unsigned int repets(std::floor(t.count()/tick_period.count()));
		for (long unsigned int i(0); i < repets ; ++i) {
			print(out);
			evolve();
			this_thread::sleep_for(tick_period);
			system("clear");
		}
	}
};



int main() {

    motifs::Motif batonh(motifs::rectangle(2,6)), batonv(batonh);
    batonv.rotate();
    motifs::Motif galaxy(batonh);
    batonh+={7,3};
    galaxy+=batonh;
    batonv+={3,0};
    galaxy+=batonv;
    batonv-={3,0};
    batonv+={0,7};
    galaxy+=batonv;
    galaxy+={10,10};


    GameOfLife<30,30> G1(galaxy,300ms);
    // GameOfLife<30,30> G2(t1), G3(t2), G4(t3);
    system("clear");
    G1.print();
    this_thread::sleep_for(1.5s);
    G1.life(30000ms);

	return 0;
}
