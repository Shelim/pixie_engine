/// USING Google Test engine

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "test/print_to.hpp"


TEST(sample, sample_always_right)
{
	EXPECT_EQ(0, 0);
}

TEST(sample, sample_always_wrong)
{
	EXPECT_EQ(5, 7);
}