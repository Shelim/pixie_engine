#include <gtest/gtest.h>
#include "utility/text/parser.hpp"
#include "print_to.hpp"


TEST(utility_text_parser, simple_parsing)
{
	EXPECT_EQ("a-b"_u, format_string("#1#-#2#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b"_u, format_string("#2#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b-a"_u, format_string("#2#-#1#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b-b"_u, format_string("#2#-#2#"_u, "a"_u, "b"_u, "c"_u));
	EXPECT_EQ("2"_u, format_string("#1#"_u, "2"_u, "b"_u, "c"_u));
	EXPECT_EQ("1"_u, format_string("#2#"_u, "a"_u, "1"_u, "c"_u));
}

TEST(utility_text_parser, formatting_comments)
{
	EXPECT_EQ("b1#"_u, format_string("#2#1#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b1#"_u, format_string("#2#1##"_u, "a"_u, "b"_u));
	EXPECT_EQ("a"_u, format_string("#1:This is sample comment#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b1#test#"_u, format_string("#2:This is sample comment#1#test#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b1#test#"_u, format_string("#2:This is sample comment#1##test#"_u, "a"_u, "b"_u));
	EXPECT_EQ("b-a-a-b-a"_u, format_string("#2#-#1:comment#-#1#-#2:comment!#-#1:2#"_u, "a"_u, "b"_u));
	EXPECT_EQ("baaba"_u, format_string("#2##1:comment##1##2:comment!##1:2#"_u, "a"_u, "b"_u));
	EXPECT_EQ("ba#aba"_u, format_string("#2##1:comment####1##2:comment!##1:2#"_u, "a"_u, "b"_u));
}

TEST(utility_text_parser, formatting_escape)
{
	EXPECT_EQ("#"_u, format_string("##"_u, "a"_u));
	EXPECT_EQ("#1#"_u, format_string("##1##"_u, "a"_u));
	EXPECT_EQ("#b"_u, format_string("###2#"_u, "a"_u, "b"_u));
	EXPECT_EQ("a#"_u, format_string("#1##"_u, "a"_u, "b"_u));
	EXPECT_EQ("b#"_u, format_string("#2:test###"_u, "a"_u, "b"_u));
}


TEST(utility_text_parser, formatting_escape)
{
	EXPECT_EQ("#"_u, format_string("##"_u, "a"_u));
	EXPECT_EQ("#1#"_u, format_string("##1##"_u, "a"_u));
	EXPECT_EQ("#b"_u, format_string("###2#"_u, "a"_u, "b"_u));
	EXPECT_EQ("a#"_u, format_string("#1##"_u, "a"_u, "b"_u));
	EXPECT_EQ("b#"_u, format_string("#2:test###"_u, "a"_u, "b"_u));
}
