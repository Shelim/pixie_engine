#include <gtest/gtest.h>
#include "utility/vfs/virtual_path.hpp"
#include "platform/path.hpp"
#include "print_to.hpp"


TEST(utility_vfs_virtual_path, type_check)
{
	engine::virtual_path_t path;
	
	path = "test/bar"_u;
	EXPECT_EQ(path.get_type(), engine::virtual_path_t::type_t::unknown);

	path = engine::virtual_path_t("test/bar"_u, engine::virtual_path_t::type_t::config);
	EXPECT_EQ(path.get_type(), engine::virtual_path_t::type_t::config);

	path.set_type(engine::virtual_path_t::type_t::modules);
	EXPECT_EQ(path.get_type(), engine::virtual_path_t::type_t::modules);
}

TEST(utility_vfs_virtual_path, capitalization_comparision)
{
	engine::virtual_path_t path1, path2;

	path1 = "test"_u;
	path2 = "TEST"_u;
	EXPECT_EQ(path1, path2);

	path1 = "TeST/BaR"_u;
	path2 = "test/Bar"_u;
	EXPECT_EQ(path1, path2);

	path1 = "TeST/BaR"_u;
	EXPECT_EQ(path1.get_path_lower(), "test/bar"_u);
}

TEST(utility_vfs_virtual_path, canonize_path)
{
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/bar/baz"), "test/bar/baz"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/bar/baz/."), "test/bar/baz"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/bar/baz/.."), "test/bar"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/../baz/.."), ""_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/../test2/baz/.."), "test2"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/test/test/../../../test2/baz/.."), "test2"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/./test/../../test2/baz/.."), "test2"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/../.."), ""_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/../.././../../."), ""_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/..test"), "test/..test"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path("test/test.."), "test/test.."_u);

}

TEST(utility_vfs_virtual_path, canonize_path_lower)
{
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("TesT/BAR/Baz"), "test/bar/baz"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("Test/Bar/Baz/."), "test/bar/baz"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("TeST/bar/BAZ/.."), "test/bar"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("Test/../BAZ/.."), ""_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("test/../TEST2/baz/.."), "test2"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("test/Test/test/../../../Test2/Baz/.."), "test2"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("test/./Test/../../TEST2/baz/.."), "test2"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("TEST/../.."), ""_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("TEST/../.././../../."), ""_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("TEST/..test"), "test/..test"_u);
	EXPECT_EQ(engine::virtual_path_t::canonize_path_lower("test/TEST.."), "test/test.."_u);

}

TEST(utility_vfs_virtual_path, resolve_path)
{
	engine::ustring_t exe_path = engine::platform::get_self_path();
	engine::ustring_t path = engine::virtual_path_t::canonize_path(exe_path.get_cstring());

	EXPECT_EQ(engine::virtual_path_t::canonize_path(engine::virtual_path_t::resolve_path("*exe_path*"_u).c_str()), path);
	EXPECT_EQ(engine::virtual_path_t::canonize_path(engine::virtual_path_t::resolve_path("*exe_path*/test/../test2/test3/../../."_u).c_str()), path);
	EXPECT_EQ(engine::virtual_path_t::canonize_path(engine::virtual_path_t::resolve_path("*exe_path*/test/.././././test5/.."_u).c_str()), path);

	engine::ustring_t path2 = path;
	path2.append("/test"_u);

	EXPECT_EQ(engine::virtual_path_t::canonize_path(engine::virtual_path_t::resolve_path("*exe_path*/test"_u).c_str()), path2);
	EXPECT_EQ(engine::virtual_path_t::canonize_path(engine::virtual_path_t::resolve_path("*exe_path*/test/../test2/.././test"_u).c_str()), path2);
	EXPECT_EQ(engine::virtual_path_t::canonize_path(engine::virtual_path_t::resolve_path("*exe_path*/test/../test2/.././test/./././."_u).c_str()), path2);
}

TEST(utility_vfs_virtual_path, ordering)
{
	engine::virtual_path_t path1, path2;

	path1 = engine::virtual_path_t("test", engine::virtual_path_t::type_t::unknown);
	path2 = engine::virtual_path_t("test", engine::virtual_path_t::type_t::common);
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test", engine::virtual_path_t::type_t::modules);
	path2 = engine::virtual_path_t("test", engine::virtual_path_t::type_t::submodules);
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test1", engine::virtual_path_t::type_t::unknown);
	path2 = engine::virtual_path_t("test2", engine::virtual_path_t::type_t::common);
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test1", engine::virtual_path_t::type_t::modules);
	path2 = engine::virtual_path_t("test2", engine::virtual_path_t::type_t::submodules);
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test2", engine::virtual_path_t::type_t::unknown);
	path2 = engine::virtual_path_t("test1", engine::virtual_path_t::type_t::common);
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test2", engine::virtual_path_t::type_t::modules);
	path2 = engine::virtual_path_t("test1", engine::virtual_path_t::type_t::submodules);
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test1");
	path2 = engine::virtual_path_t("test2");
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test4/../test2/test1");
	path2 = engine::virtual_path_t("test3/test2/test3");
	EXPECT_LT(path1, path2);

	path1 = engine::virtual_path_t("test5/../test2");
	path2 = engine::virtual_path_t("test3");
	EXPECT_LT(path1, path2);
}

TEST(utility_vfs_virtual_path, subitem)
{
	engine::virtual_path_t path;

	path = "test/test2"_u;
	path = path.subitem("test3"_u);
	EXPECT_EQ(path, "test/test2/test3"_u);

	path = "test/test2/../test3/.."_u;
	path = path.subitem("test3"_u);
	EXPECT_EQ(path, "test/test3"_u);

	path = "test/test2/../test3/.."_u;
	path = path.subitem("test3/.."_u);
	EXPECT_EQ(path, "test"_u);

	path = "test/test2/../test3/.."_u;
	path = path.subitem("test3/.././test4/."_u);
	EXPECT_EQ(path, "test/test4"_u);

	path = "test/test2/../test3/.."_u;
	path = path.subitem("evil/../../."_u);
	EXPECT_EQ(path, "test"_u);

	path = "test"_u;
	path = path.subitem("../../."_u);
	EXPECT_EQ(path, "test"_u);

	path = "test"_u;
	path = path.subitem("test3"_u);
	EXPECT_EQ(path, "test/test3"_u);

}