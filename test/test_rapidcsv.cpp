#include "rapidcsv.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    //rapidcsv::Document doc(std::string(TEST_PATH)+"/test.csv", rapidcsv::LabelParams(-1,-1));
    //std::cout << doc.GetRowCount() << " " << doc.GetColumnCount() << std::endl;
    //cout << doc.GetRow() << endl;

    rapidcsv::Document lievres(std::string(LOCAL_PATH)+"/sims/sim-lievres.csv", rapidcsv::LabelParams(-1,0));
    //std::vector<string> tab(lievres.GetRow<string>(2));
    //std::copy(tab.begin(), tab.end(), std::ostream_iterator<string>(std::cout, ","));
    //std::cout << lievres.GetCell<size_t>(18,2);
    string s("");
    return 0;
}