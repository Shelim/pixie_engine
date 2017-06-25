/// USING Google Test engine
#if 0

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "common/utility/text/ustring.hpp"
#include "common/asset/path.hpp"
#include "common/utility/vfs/virtual_path.hpp"
#include "test/print_to.hpp"

#define EXPECT_CLEAN_EQ(val, res) { engine::ustring_t item = engine::asset::path_t::clean_path(_U(val)); EXPECT_EQ(item, _U(res)); }

TEST(asset_path, clean_basics)
{
	EXPECT_CLEAN_EQ("/test/path.exe", "test/path.exe");
	EXPECT_CLEAN_EQ("no_seps_end", "");
	EXPECT_CLEAN_EQ("this/is/sample/path", "is/sample/path");
	EXPECT_CLEAN_EQ("this/is/sample/path.bmp", "is/sample/path.bmp");
	EXPECT_CLEAN_EQ("/root/path", "root/path");
	EXPECT_CLEAN_EQ("ending_in_slash/", "");
	EXPECT_CLEAN_EQ("/test/", "test/");
}

TEST(asset_path, clean_delete)
{
	EXPECT_CLEAN_EQ("/test/!path.exe", "test/path.exe");
	EXPECT_CLEAN_EQ("no_!seps_end", "");
	EXPECT_CLEAN_EQ("this/!is/sample/path", "is/sample/path");
	EXPECT_CLEAN_EQ("this/is/!sample/path.bmp", "is/sample/path.bmp");
	EXPECT_CLEAN_EQ("!/root/path", "root/path");
	EXPECT_CLEAN_EQ("ending_in_slash/!", "");
	EXPECT_CLEAN_EQ("/test!/", "test/");
}

#define EXPECT_CLEAN_COMMON_EQ(val, res) { engine::ustring_t item = engine::asset::path_t::clean_path(_U(val), true); EXPECT_EQ(item, _U(res)); }

TEST(asset_path, clean_common_basics)
{
	EXPECT_CLEAN_COMMON_EQ("/test/path.exe", "/test/path.exe");
	EXPECT_CLEAN_COMMON_EQ("no_seps_end", "no_seps_end");
	EXPECT_CLEAN_COMMON_EQ("this/is/sample/path", "this/is/sample/path");
	EXPECT_CLEAN_COMMON_EQ("this/is/sample/path.bmp", "this/is/sample/path.bmp");
	EXPECT_CLEAN_COMMON_EQ("/root/path", "/root/path");
	EXPECT_CLEAN_COMMON_EQ("ending_in_slash/", "ending_in_slash/");
	EXPECT_CLEAN_COMMON_EQ("/test/", "/test/");
}

TEST(asset_path, clean_common_delete)
{
	EXPECT_CLEAN_COMMON_EQ("/test/!path.exe", "/test/path.exe");
	EXPECT_CLEAN_COMMON_EQ("no_!seps_end", "no_seps_end");
	EXPECT_CLEAN_COMMON_EQ("this/!is/sample/path", "this/is/sample/path");
	EXPECT_CLEAN_COMMON_EQ("this/is/!sample/path.bmp", "this/is/sample/path.bmp");
	EXPECT_CLEAN_COMMON_EQ("!/root/path", "/root/path");
	EXPECT_CLEAN_COMMON_EQ("ending_in_slash/!", "ending_in_slash/");
	EXPECT_CLEAN_COMMON_EQ("/test!/", "/test/");
}


#define EXPECT_PATH_EQ(path, test, result) { engine::asset::path_t item (engine::virtual_path_t(path, engine::virtual_path_t::type_t::modules)); EXPECT_EQ(item.get_##test(), _U(result)); }

TEST(asset_path, info_t_owner)
{
	EXPECT_PATH_EQ("sample/test/test2", owner, "sample");
	EXPECT_PATH_EQ("sample2/test/test2", owner, "sample2");
	EXPECT_PATH_EQ("sample3/test/test3.exe", owner, "sample3");
	EXPECT_PATH_EQ("/test/test3.exe", owner, "");
	EXPECT_PATH_EQ("/test", owner, "");
	EXPECT_PATH_EQ("", owner, "");
	EXPECT_PATH_EQ("sample/", owner, "sample");
	EXPECT_PATH_EQ("/", owner, "");
}

TEST(asset_path, info_t_owner_delete)
{
	EXPECT_PATH_EQ("sam!ple/test/test2", owner, "sam!ple");
	EXPECT_PATH_EQ("sample2/!test!/test2", owner, "sample2");
	EXPECT_PATH_EQ("sample3!/test/test3.exe", owner, "sample3!");
	EXPECT_PATH_EQ("/!test/!test3.exe", owner, "");
	EXPECT_PATH_EQ("!/test", owner, "!");
	EXPECT_PATH_EQ("!!!/!!!!", owner, "!!!");
	EXPECT_PATH_EQ("sample/!", owner, "sample");
	EXPECT_PATH_EQ("/!", owner, "");
}

TEST(asset_path, info_t_tag_simple)
{
	EXPECT_PATH_EQ("sample/^test/test2", tag, "test");
	EXPECT_PATH_EQ("sample2/te^st/test2", tag, "st");
	EXPECT_PATH_EQ("sample3/test/^test3.exe", tag, "test3");
	EXPECT_PATH_EQ("/test/tes^t3.exe", tag, "t3");
	EXPECT_PATH_EQ("/^test", tag, "test");
	EXPECT_PATH_EQ("/te^st", tag, "st");
	EXPECT_PATH_EQ("sam^ple/", tag, "");
	EXPECT_PATH_EQ("/^", tag, "");
	EXPECT_PATH_EQ("^/", tag, "");
	EXPECT_PATH_EQ("test/^/", tag, "");
}

TEST(asset_path, info_t_tag_multiple)
{
	EXPECT_PATH_EQ("sample/^test/te^st2", tag, "st2");
	EXPECT_PATH_EQ("sample2/te^st/^test2", tag, "test2");
	EXPECT_PATH_EQ("sample3/test/^te^st3.exe", tag, "st3");
	EXPECT_PATH_EQ("/test/t^es^t3.exe", tag, "t3");
	EXPECT_PATH_EQ("/^test^", tag, "");
	EXPECT_PATH_EQ("/te^^st", tag, "st");
	EXPECT_PATH_EQ("sam^ple/^", tag, "");
	EXPECT_PATH_EQ("/^test^", tag, "");
	EXPECT_PATH_EQ("^/test^", tag, "");
	EXPECT_PATH_EQ("te^st/^test/a^b", tag, "b");
}

TEST(asset_path, info_t_lang)
{
	EXPECT_PATH_EQ("sample/test/test2#pl_PL", lang, "pl_pl");
	EXPECT_PATH_EQ("sample2/test/#test2", lang, "test2");
	EXPECT_PATH_EQ("sample3/#test/test3.exe", lang, "test");
	EXPECT_PATH_EQ("/t#est/test3.exe", lang, "est");
	EXPECT_PATH_EQ("/test#", lang, "");
	EXPECT_PATH_EQ("module/#blah.exe", lang, "blah");
	EXPECT_PATH_EQ("sample/b#lah.exe", lang, "lah");
	EXPECT_PATH_EQ("mod#blah/test", lang, "");
}

TEST(asset_path, info_t_lang_multiple)
{
	EXPECT_PATH_EQ("sample/#test/test2#pl_PL", lang, "pl_pl");
	EXPECT_PATH_EQ("sample2/#test/#test2", lang, "test2");
	EXPECT_PATH_EQ("sa#mple3/#test/test3.exe", lang, "test");
	EXPECT_PATH_EQ("/t#est/tes#t3.exe", lang, "t3");
	EXPECT_PATH_EQ("#/test#", lang, "");
	EXPECT_PATH_EQ("mod#ule/#blah.exe", lang, "blah");
	EXPECT_PATH_EQ("#sample/b#lah.exe", lang, "lah");
	EXPECT_PATH_EQ("mod#blah/#test", lang, "test");
}

TEST(asset_path, info_t_lang_tag)
{
	EXPECT_PATH_EQ("sample/#te^st/test2#pl^_PL", lang, "pl");
	EXPECT_PATH_EQ("sample/#te^st/test2#pl^_PL", tag, "_pl");
	EXPECT_PATH_EQ("sample2/#te^st/#test2", lang, "test2");
	EXPECT_PATH_EQ("sample2/#te^st/#test2", tag, "st");
	EXPECT_PATH_EQ("sa#mp^le3/#test/test3.exe", lang, "test");
	EXPECT_PATH_EQ("sa#mp^le3/#test/test3.exe", tag, "");
	EXPECT_PATH_EQ("/t#est/^tes#t3.exe", lang, "t3");
	EXPECT_PATH_EQ("/t#est/^tes#t3.exe", tag, "tes");
	EXPECT_PATH_EQ("#/te^st#", lang, "");
	EXPECT_PATH_EQ("#/te^st#", tag, "st");
	EXPECT_PATH_EQ("tag/tag^#", lang, "");
	EXPECT_PATH_EQ("tag/tag^#", tag, "");
	EXPECT_PATH_EQ("tag/tag^#.exe", lang, "");
	EXPECT_PATH_EQ("tag/tag^#.exe", tag, "");
	EXPECT_PATH_EQ("#sample/b#la^h.exe", lang, "la");
	EXPECT_PATH_EQ("#sample/b#la^h.exe", tag, "h");
	EXPECT_PATH_EQ("mod#blah/#test.^exe", lang, "test");
	EXPECT_PATH_EQ("mod#blah/#test.^exe", tag, "exe");
}



#define EXPECT_PATH_COMMON_EQ(path, test, result) { engine::asset::path_t item (engine::virtual_path_t(path, engine::virtual_path_t::type_t::common)); EXPECT_EQ(item.get_##test(), _U(result)); }

TEST(asset_path, info_t_common_owner)
{
	EXPECT_PATH_COMMON_EQ("sample/test/test2", owner, "");
	EXPECT_PATH_COMMON_EQ("sample2/test/test2", owner, "");
	EXPECT_PATH_COMMON_EQ("sample3/test/test3.exe", owner, "");
	EXPECT_PATH_COMMON_EQ("/test/test3.exe", owner, "");
	EXPECT_PATH_COMMON_EQ("/test", owner, "");
	EXPECT_PATH_COMMON_EQ("", owner, "");
	EXPECT_PATH_COMMON_EQ("sample/", owner, "");
	EXPECT_PATH_COMMON_EQ("/", owner, "");
}

TEST(asset_path, info_t_common_owner_delete)
{
	EXPECT_PATH_COMMON_EQ("sam!ple/test/test2", owner, "");
	EXPECT_PATH_COMMON_EQ("sample2/!test!/test2", owner, "");
	EXPECT_PATH_COMMON_EQ("sample3!/test/test3.exe", owner, "");
	EXPECT_PATH_COMMON_EQ("/!test/!test3.exe", owner, "");
	EXPECT_PATH_COMMON_EQ("!/test", owner, "");
	EXPECT_PATH_COMMON_EQ("!!!/!!!!", owner, "");
	EXPECT_PATH_COMMON_EQ("sample/!", owner, "");
	EXPECT_PATH_COMMON_EQ("/!", owner, "");
}

TEST(asset_path, info_t_common_tag_simple)
{
	EXPECT_PATH_COMMON_EQ("sample/^test/test2", tag, "test");
	EXPECT_PATH_COMMON_EQ("sample2/te^st/test2", tag, "st");
	EXPECT_PATH_COMMON_EQ("sample3/test/^test3.exe", tag, "test3");
	EXPECT_PATH_COMMON_EQ("/test/tes^t3.exe", tag, "t3");
	EXPECT_PATH_COMMON_EQ("/^test", tag, "test");
	EXPECT_PATH_COMMON_EQ("/te^st", tag, "st");
	EXPECT_PATH_COMMON_EQ("sam^ple/", tag, "ple");
	EXPECT_PATH_COMMON_EQ("/^", tag, "");
	EXPECT_PATH_COMMON_EQ("^/", tag, "");
	EXPECT_PATH_COMMON_EQ("test/^/", tag, "");
}

TEST(asset_path, info_t_common_tag_multiple)
{
	EXPECT_PATH_COMMON_EQ("sample/^test/te^st2", tag, "st2");
	EXPECT_PATH_COMMON_EQ("sample2/te^st/^test2", tag, "test2");
	EXPECT_PATH_COMMON_EQ("sample3/test/^te^st3.exe", tag, "st3");
	EXPECT_PATH_COMMON_EQ("/test/t^es^t3.exe", tag, "t3");
	EXPECT_PATH_COMMON_EQ("/^test^", tag, "");
	EXPECT_PATH_COMMON_EQ("/te^^st", tag, "st");
	EXPECT_PATH_COMMON_EQ("sam^ple/^", tag, "");
	EXPECT_PATH_COMMON_EQ("/^test^", tag, "");
	EXPECT_PATH_COMMON_EQ("^/test^", tag, "");
	EXPECT_PATH_COMMON_EQ("te^st/^test/a^b", tag, "b");
}

TEST(asset_path, info_t_common_lang)
{
	EXPECT_PATH_COMMON_EQ("sample/test/test2#pl_PL", lang, "pl_pl");
	EXPECT_PATH_COMMON_EQ("sample2/test/#test2", lang, "test2");
	EXPECT_PATH_COMMON_EQ("sample3/#test/test3.exe", lang, "test");
	EXPECT_PATH_COMMON_EQ("/t#est/test3.exe", lang, "est");
	EXPECT_PATH_COMMON_EQ("/test#", lang, "");
	EXPECT_PATH_COMMON_EQ("module/#blah.exe", lang, "blah");
	EXPECT_PATH_COMMON_EQ("sample/b#lah.exe", lang, "lah");
	EXPECT_PATH_COMMON_EQ("mod#blah/test", lang, "blah");
}

TEST(asset_path, info_t_common_lang_multiple)
{
	EXPECT_PATH_COMMON_EQ("sample/#test/test2#pl_PL", lang, "pl_pl");
	EXPECT_PATH_COMMON_EQ("sample2/#test/#test2", lang, "test2");
	EXPECT_PATH_COMMON_EQ("sa#mple3/#test/test3.exe", lang, "test");
	EXPECT_PATH_COMMON_EQ("/t#est/tes#t3.exe", lang, "t3");
	EXPECT_PATH_COMMON_EQ("#/test#", lang, "");
	EXPECT_PATH_COMMON_EQ("mod#ule/#blah.exe", lang, "blah");
	EXPECT_PATH_COMMON_EQ("#sample/b#lah.exe", lang, "lah");
	EXPECT_PATH_COMMON_EQ("mod#blah/#test", lang, "test");
}

TEST(asset_path, info_t_common_lang_tag)
{
	EXPECT_PATH_COMMON_EQ("sample/#te^st/test2#pl^_PL", lang, "pl");
	EXPECT_PATH_COMMON_EQ("sample/#te^st/test2#pl^_PL", tag, "_pl");
	EXPECT_PATH_COMMON_EQ("sample2/#te^st/#test2", lang, "test2");
	EXPECT_PATH_COMMON_EQ("sample2/#te^st/#test2", tag, "st");
	EXPECT_PATH_COMMON_EQ("sa#mp^le3/#test/test3.exe", lang, "test");
	EXPECT_PATH_COMMON_EQ("sa#mp^le3/#test/test3.exe", tag, "le3");
	EXPECT_PATH_COMMON_EQ("/t#est/^tes#t3.exe", lang, "t3");
	EXPECT_PATH_COMMON_EQ("/t#est/^tes#t3.exe", tag, "tes");
	EXPECT_PATH_COMMON_EQ("#/te^st#", lang, "");
	EXPECT_PATH_COMMON_EQ("#/te^st#", tag, "st");
	EXPECT_PATH_COMMON_EQ("tag/tag^#", lang, "");
	EXPECT_PATH_COMMON_EQ("tag/tag^#", tag, "");
	EXPECT_PATH_COMMON_EQ("tag/tag^#.exe", lang, "");
	EXPECT_PATH_COMMON_EQ("tag/tag^#.exe", tag, "");
	EXPECT_PATH_COMMON_EQ("#sample/b#la^h.exe", lang, "la");
	EXPECT_PATH_COMMON_EQ("#sample/b#la^h.exe", tag, "h");
	EXPECT_PATH_COMMON_EQ("mod#blah/#test.^exe", lang, "test");
	EXPECT_PATH_COMMON_EQ("mod#blah/#test.^exe", tag, "exe");
}

#endif