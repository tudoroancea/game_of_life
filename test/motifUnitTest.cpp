//
// Created by Tudor Oancea on 28/03/2021.
//

#include "gtest/gtest.h"
#include "Motif.hpp"

#include <filesystem>

using namespace std;
using namespace filesystem;

TEST(MotifTest, Construction) {
	Motif M1;
	ASSERT_TRUE(M1.empty());
	
	Motif M2(segment(0,0,10,10));
	for (size_t i(0); i <= 10; ++i) {
		ASSERT_EQ(M2[i].first,i);
		ASSERT_EQ(M2[i].second,i);
	}
	
	Motif M3("gland.csv", presaved);
	
	path p(LOCAL_PATH);
	p /= "motifs/lievres.csv";
	if (exists(p)) {
		Motif M4(p);
	}
}

TEST(MotifTest, Access) {

}

TEST(MotifTest, Modification) {
	Motif M1(segment(0,0,10,10));
	ASSERT_EQ(M1.size(), 11);
	
}