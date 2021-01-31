#include "rapidcsv.h"
#include <iostream>

using namespace std;

int main() {
    rapidcsv::Document doc(std::string(TEST_PATH)+"/test.csv", rapidcsv::LabelParams(-1,-1));
    std::cout << doc.GetRowCount() << " " << doc.GetColumnCount() << std::endl;
    cout << doc.GetRow() << endl;

    //rapidcsv::Document lievres(std::string(LOCAL_PATH)+"/sims/sim-lievres.csv", rapidcsv::LabelParams(-1,0));
    //std::cout << lievres.GetCell<unsigned int>(0,"dimensions (lignes x colonnes)") << std::endl;
    return 0;
}