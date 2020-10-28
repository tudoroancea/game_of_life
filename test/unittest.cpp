#include <iostream>
#include "motifs.h"
#include "game_of_life.h"
#include <algorithm>
#include <vector>
#include <iterator>

int main() {
    std::vector<std::string> L(existing_local_motifs());
    std::copy(L.begin(), L.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    return 0;
}