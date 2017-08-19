#include <gtest/gtest.h>
#include "utility/text/expand.hpp"
#include "print_to.hpp"


TEST(utility_text_expand, vector_expanding)
{
	std::vector<engine::ustring_t> result;
	engine::expand_into_vector(result, 5, true, 0.1, "test"_u);

	EXPECT_EQ(result[0], engine::to_string(5));
	EXPECT_EQ(result[1], engine::to_string(true));
	EXPECT_EQ(result[2], engine::to_string(0.1));
	EXPECT_EQ(result[3], engine::to_string("test"_u));
}

TEST(utility_text_expand, array_expanding)
{
	std::array<engine::ustring_t, 4> result;
	engine::expand_into_array(result, 5, true, 0.1, "test"_u);

	EXPECT_EQ(result[0], engine::to_string(5));
	EXPECT_EQ(result[1], engine::to_string(true));
	EXPECT_EQ(result[2], engine::to_string(0.1));
	EXPECT_EQ(result[3], engine::to_string("test"_u));
}