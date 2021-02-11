# Connected Components counting

## Direct CC counting

Basic general recursive version to find the number of CC:
```cpp
// direction vectors
const std::array<int,8> dx({1,1,1,0,0,-1,-1,-1});
const std::array<int,8> dy({1,0,-1,1,-1,1,0, -1});

// the input matrix
std::array<std::array<bool,MAX_LIGNES>,MAX_COLONNES> grille;

// the labels, 0 means unlabeled
std::array<std::array<size_t,MAX_LIGNES>,MAX_COLONNES> labels;
for (auto& line : labels) {
    for (auto& cell : line) {
        cell = 0;
    }
}

// Depth-first search : we recursively label each cell of the connected component
void dfs(size_t x, size_t y, size_t current_label) {
    if (x < MAX_LIGNES && y < MAX_COLONNES && grille[x][y] && labels[x][y] == 0) {
        // mark the current cell
        labels[x][y] = current_label;

        // recursively mark the neighbors
        for (int direction = 0; direction < 8; ++direction) {
            dfs(x + dx[direction], y + dy[direction], current_label);
        }
    }
}

size_t find_components() {
    size_t current_label(0);
    for (size_t i(0); i < MAX_LIGNES ; ++i) {
        for (size_t j(0); j < MAX_COLONNES ; ++j) {
            if (grille[i][j] && labels[i][j] == 0) {
                dfs(i,j,++current_label);
            }
        }
    }
    return current_label;
}
```

Basic general iterative version to find the number of CC:
```cpp
// direction vectors
std::array<int,8> dx({1,1,1,0,0,-1,-1,-1});
std::array<int,8> dy({1,0,-1,1,-1,1,0, -1});

// the input matrix
std::array<std::array<bool,MAX_LIGNES>,MAX_COLONNES> grille;

// the labels, 0 means unlabeled
std::array<std::array<size_t,MAX_LIGNES>,MAX_COLONNES> labels;
for (auto const& line : labels) {
    for (auto const& cell : line) {
        cell = 0;
    }
}
// to store the connected cells to examine next
std::queue<coord> q;

size_t find_components() {
    size_t current_label(0);
    for (size_t i(0); i < MAX_LIGNES ; ++i) {
        for (size_t j(0); j < MAX_COLONNES ; ++j) {
            if (grille[i][j] && labels[i][j] == 0) {
                ++current_label;
                labels[i][j] = current_label;
                q.push({i,j});
            } else {
                continue;
            }
            while (!q.empty()) {
                coord c(q.front());
                q.pop();
                for (size_t direction(0); direction < 8; ++direction) {
                    if (X(c)+dx[direction] < MAX_LIGNES
                    && Y(c)+dy[direction] < MAX_COLONNES
                    && grille[X(c)+dx[direction]][Y(c)+dy[direction]]
                    && labels[X(c)+dx[direction]][Y(c)+dy[direction]] == 0) {
                        labels[X(c)+dx[direction]][Y(c)+dy[direction]] = current_label;
                        q.push({X(c)+dx[direction],Y(c)+dy[direction]});
                    }
                }
            }
        }
    }
    return current_label;
}
```

Final implemented recursive version
```cpp
void GameOfLife::dfs(std::unordered_map<coord, size_t, pair_hash>& labels, size_t x, size_t y, size_t label) const {
	if (access(x,y) && labels[{x,y}] == 0) {
		// mark the current cell
        labels[{x,y}] = label;

        // recursively mark the neighbors
        for (int direction = 0; direction < 8; ++direction) {
            dfs(labels, x + dx[direction], y + dy[direction], label);
        }
	}
}
size_t GameOfLife::nbr_CC_rec() const {
	std::unordered_map<coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	for (auto const& cell : vivantes) {
		if (access(X(cell), Y(cell)) && labels[cell] == 0) {
			dfs(labels, X(cell), Y(cell), ++current_label);
		}
	}
	return current_label;
}
```
Final implemented iterative version
```cpp
size_t GameOfLife::nbr_CC_ite() const {
	std::unordered_map<coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	std::queue<coord> q;
	for(auto const& cell : vivantes) {
		if (access(X(cell), Y(cell)) && labels[cell] == 0) {
			++current_label;
			labels[cell] = current_label;
			q.push(cell);
		} else {
			continue;
		}
		while (!q.empty()) {
			coord c(q.front());
			q.pop();
			for (size_t direction(0); direction < 8; ++direction) {
				if (access(X(c)+dx[direction], Y(c)+dy[direction]) && labels[{X(c)+dx[direction], Y(c)+dy[direction]}] == 0) {
					labels[{X(c)+dx[direction], Y(c)+dy[direction]}] = current_label;
					q.push({X(c)+dx[direction], Y(c)+dy[direction]});
				}
			}
		}
	}
	return current_label;
}
To get the actual components we can just add at the end of each algorithm
```cpp
std::vector<Motif> res(current_label);
for (size_t i(0); i < current_label ; ++i) {
    for (auto const& pair : labels) {
        if (pair.second == i+1) {
            res[i].push_back(pair.first);
        }
    }
}
```
and replace `return current_label;` by `return res;`

## 2-pass CC counting

Two-pass algorithm, aka *Hoshen–Kopelman algorithm* :
```cpp
// the input matrix
std::array<std::array<bool,MAX_LIGNES>,MAX_COLONNES> grille;

// the labels, 0 means unlabeled
std::array<std::array<size_t,MAX_LIGNES>,MAX_COLONNES> labels;
for (auto const& line : labels) {
    for (auto const& cell : line) {
        cell = 0;
    }
}


size_t two_pass() {
    size_t NextLabel(1);
    SimpleEquivalenceTable eqtable(1);
    // 1st pass
    for (size_t i(0); i < MAX_LIGNES ; ++i) {
        for (size_t j(0); j < MAX_COLONNES ; ++j) {
            if (grille[i][j]) {
                // we find the living neighbors
                std::vector<coord> neighbors;
                for (size_t direction(0); direction < 8 ; ++direction) {
                    if (i+dx[direction] < MAX_LIGNES && j+dy[direction] < MAX_COLONNES && grille[i+dx[direction]][j+dy[direction]]) {
                        neighbors.push_back({i+dx[direction], j+dy[direction]});
                    }
                }
                // On analyse ces voisins
                if (neighbors.empty()) {
                    eqtable.make_set();
                    labels[i][j] = NextLabel;
                    ++NextLabel;
                } else {
                    // On associe à (i,j) le plus petit label de ses voisins et on crée des équivalences entre (i,j) et ses voisins
                    size_t min(0);
                    for (auto const& cell : neighbors) if (labels[X(cell)][Y(cell)] < min) min = labels[X(cell)][Y(cell)];
                    labels[i][j] = min;
                    for (auto const& cell : neighbors) eqtable.unionn(min, labels[X(cell)][Y(cell)]);
                }
            }
        }
    }

    // 2nd pass:
    for (size_t i(0); i < MAX_LIGNES ; ++i) {
        for (size_t j(0); j < MAX_COLONNES ; ++j) {
            if (grille[i][j]) {
                labels[i][j] = eqtable.find(labels[i][j]);
            }
        }
    }
    return eqtable.nbr_classes();
}
```
