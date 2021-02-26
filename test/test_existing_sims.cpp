#include "GameOfLife.hpp"
#include "Motif.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
using namespace std;

int main() {
    vector<string> v(existing_presaved_sims()), u(existing_local_sims());
    copy(v.begin(), v.end(), ostream_iterator<string>(cout, "\n"));
    copy(u.begin(), u.end(), ostream_iterator<string>(cout, "\n"));
    return 0;
}