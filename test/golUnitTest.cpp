//
// Created by Tudor Oancea on 28/03/2021.
//

#include <gtest/gtest.h>
#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "termcolor.hpp"

#include <filesystem>

// Tests for class GameOfLife ============================================================================================

TEST(GameOfLifeTest, Construction) {
	auto G(new GameOfLife);
	std::cout << G->vivantes().size() << std::endl;
	static GameOfLife G2;
	auto v(new GameOfLifeView);
	delete G;
	delete v;
	
//	GameOfLife G2(Motif({{1,1}}));
	ASSERT_EQ(1,1);
}

// Tests for class GameOfLifeView ============================================================================================

// Tests for class Simulation ============================================================================================