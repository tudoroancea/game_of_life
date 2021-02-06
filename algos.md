Basic recursive version to find the number of CC:
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
    if (x < MAX_LIGNES && y < MAX_COLONNES && labels[x][y] == 0) {
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


Basic iterative version to find the number of CC:
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
            ++current_label;
            if (grille[i][j] && labels[i][j] == 0) {
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
                if (neighbors.empty()) {
                    eqtable.make_set();
                    labels[i][j] = NextLabel;
                    ++NextLabel;
                } else {
                    std::vector<size_t> L(neighbors.size());
                    for (size_t k(0); k < neighbors.size(); ++k) {L[k] = labels[X(neighbors[k])][Y(neighbors[k])];}
                    size_t min(0);
                    for (auto const& m : L) if (m < min) min = m;
                    labels[i][j] = min;
                    for (auto const& m : L) eqtable.unionn(m,min);
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
