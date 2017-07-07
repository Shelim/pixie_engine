#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "utility/text/ustring.hpp"
#include "print_to.hpp"

TEST(ustring, formatting)
{
	EXPECT_EQ(engine::format_utf8(u8"#1#-#2#", "a"_u, "b"_u), "a-b"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2#", "a"_u, "b"_u), "b"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2#-#1#", "a"_u, "b"_u), "b-a"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2#-#2#", "a"_u, "b"_u, "c"_u), "b-b"_u);
	EXPECT_EQ(engine::format_utf8(u8"#1#", "2"_u, "b"_u, "c"_u), "2"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2#", "a"_u, "1"_u, "c"_u), "1"_u);
}

TEST(ustring, formatting_comments)
{
	EXPECT_EQ(engine::format_utf8(u8"#2#1#", "a"_u, "b"_u), "b1"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2#1##", "a"_u, "b"_u), "b1#"_u);
	EXPECT_EQ(engine::format_utf8(u8"#1:This is sample comment#", "a"_u, "b"_u), "a"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2:This is sample comment#1#test#", "a"_u, "b"_u), "b1"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2:This is sample comment#1##test#", "a"_u, "b"_u), "b1#test"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2#-#1:comment#-#1#-#2:comment!#-#1:2#", "a"_u, "b"_u), "b-a-a-b-a"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2##1:comment##1##2:comment!##1:2#", "a"_u, "b"_u), "baaba"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2##1:comment####1##2:comment!##1:2#", "a"_u, "b"_u), "ba#aba"_u);
}

TEST(ustring, formatting_escape)
{
	EXPECT_EQ(engine::format_utf8(u8"##", "a"_u), "#"_u);
	EXPECT_EQ(engine::format_utf8(u8"##1##", "a"_u), "#1#"_u);
	EXPECT_EQ(engine::format_utf8(u8"###2#", "a"_u, "b"_u), "#b"_u);
	EXPECT_EQ(engine::format_utf8(u8"#1##", "a"_u, "b"_u), "a"_u);
	EXPECT_EQ(engine::format_utf8(u8"#2:test###", "a"_u, "b"_u), "b#"_u);
}
