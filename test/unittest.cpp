//
// Created by Tudor Oancea on 21/03/2021.
//

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
	EXPECT_EQ(3,1);
	bool a(true);
	EXPECT_FALSE(a);
}

TEST(HelloTest, NewTest) {
	EXPECT_EQ(1+1,2);
}