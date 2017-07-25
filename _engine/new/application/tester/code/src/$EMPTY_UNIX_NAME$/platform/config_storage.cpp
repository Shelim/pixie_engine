#include <gtest/gtest.h>
#include <cstdint>
#include "platform/config_storage.hpp"
#include "print_to.hpp"

#define TEST_STORE_AND_RETRIEVE(name, x, def) engine::platform::store(name##_u, x); EXPECT_EQ(x, engine::platform::retrieve(name##_u, def));

TEST(platform_config_storage, storing_zeros)
{
	int8_t i8 = 0; TEST_STORE_AND_RETRIEVE("test", i8, (int8_t) 1);
	int16_t i16 = 0; TEST_STORE_AND_RETRIEVE("test", i16, (int16_t) 1);
	int32_t i32 = 0; TEST_STORE_AND_RETRIEVE("test", i32, (int32_t) 1);
	int64_t i64 = 0; TEST_STORE_AND_RETRIEVE("test", i64, (int64_t) 1);
	uint8_t u8 = 0; TEST_STORE_AND_RETRIEVE("test", u8, (uint8_t) 1);
	uint16_t u16 = 0; TEST_STORE_AND_RETRIEVE("test", u16, (uint16_t) 1);
	uint32_t u32 = 0; TEST_STORE_AND_RETRIEVE("test", u32, (uint32_t) 1);
	uint64_t u64 = 0; TEST_STORE_AND_RETRIEVE("test", u64, (uint64_t) 1);
}

TEST(platform_config_storage, storing_positive)
{
	int8_t i8 = 77; TEST_STORE_AND_RETRIEVE("test", i8, (int8_t)1);
	int16_t i16 = 77; TEST_STORE_AND_RETRIEVE("test", i16, (int16_t)1);
	int32_t i32 = 77; TEST_STORE_AND_RETRIEVE("test", i32, (int32_t)1);
	int64_t i64 = 77; TEST_STORE_AND_RETRIEVE("test", i64, (int64_t)1);
	uint8_t u8 = 77; TEST_STORE_AND_RETRIEVE("test", u8, (uint8_t)1);
	uint16_t u16 = 77; TEST_STORE_AND_RETRIEVE("test", u16, (uint16_t)1);
	uint32_t u32 = 77; TEST_STORE_AND_RETRIEVE("test", u32, (uint32_t)1);
	uint64_t u64 = 77; TEST_STORE_AND_RETRIEVE("test", u64, (uint64_t)1);
}

TEST(platform_config_storage, storing_negative)
{
	int8_t i8 = -77; TEST_STORE_AND_RETRIEVE("test", i8, (int8_t)1);
	int16_t i16 = -77; TEST_STORE_AND_RETRIEVE("test", i16, (int16_t)1);
	int32_t i32 = -77; TEST_STORE_AND_RETRIEVE("test", i32, (int32_t)1);
	int64_t i64 = -77; TEST_STORE_AND_RETRIEVE("test", i64, (int64_t)1);
	uint8_t u8 = 77; TEST_STORE_AND_RETRIEVE("test", u8, (uint8_t)1);
	uint16_t u16 = 77; TEST_STORE_AND_RETRIEVE("test", u16, (uint16_t)1);
	uint32_t u32 = 77; TEST_STORE_AND_RETRIEVE("test", u32, (uint32_t)1);
	uint64_t u64 = 77; TEST_STORE_AND_RETRIEVE("test", u64, (uint64_t)1);
}

TEST(platform_config_storage, storing_large_positive)
{
	int8_t i8 = 1 << 6; TEST_STORE_AND_RETRIEVE("test", i8, (int8_t)1);
	int16_t i16 = 1 << 14; TEST_STORE_AND_RETRIEVE("test", i16, (int16_t)1);
	int32_t i32 = 1 << 30; TEST_STORE_AND_RETRIEVE("test", i32, (int32_t)1);
	int64_t i64 = 1ll << 62; TEST_STORE_AND_RETRIEVE("test", i64, (int64_t)1);
	uint8_t u8 = 1 << 7; TEST_STORE_AND_RETRIEVE("test", u8, (uint8_t)1);
	uint16_t u16 = 1 << 15; TEST_STORE_AND_RETRIEVE("test", u16, (uint16_t)1);
	uint32_t u32 = 1 << 31; TEST_STORE_AND_RETRIEVE("test", u32, (uint32_t)1);
	uint64_t u64 = 1ll << 63; TEST_STORE_AND_RETRIEVE("test", u64, (uint64_t)1);
}

TEST(platform_config_storage, storing_large_negative)
{
	int8_t i8 = 0x80; TEST_STORE_AND_RETRIEVE("test", i8, (int8_t)1);
	int16_t i16 = 0x8000; TEST_STORE_AND_RETRIEVE("test", i16, (int16_t)1);
	int32_t i32 = 0x80000000; TEST_STORE_AND_RETRIEVE("test", i32, (int32_t)1);
	int64_t i64 = 0x8000000000000000ll; TEST_STORE_AND_RETRIEVE("test", i64, (int64_t)1);
}

TEST(platform_config_storage, boolean_values)
{
	bool t = true; TEST_STORE_AND_RETRIEVE("test", t, false);
	bool f = false; TEST_STORE_AND_RETRIEVE("test", f, true);
}

TEST(platform_config_storage, ustring)
{
	TEST_STORE_AND_RETRIEVE("test", "foo_bar"_u, "bar_foo"_u);
	TEST_STORE_AND_RETRIEVE("test", "123456789"_u, "987654321"_u);
	TEST_STORE_AND_RETRIEVE("test", ""_u, "non_empty"_u);
	TEST_STORE_AND_RETRIEVE("test", "non_empty"_u, ""_u);
}

TEST(platform_config_storage, non_existing)
{
	EXPECT_EQ((int8_t)77, engine::platform::retrieve("_non_existing"_u, (int8_t)77));
	EXPECT_EQ((int16_t)77, engine::platform::retrieve("_non_existing"_u, (int16_t)77));
	EXPECT_EQ((int32_t)77, engine::platform::retrieve("_non_existing"_u, (int32_t)77));
	EXPECT_EQ((int64_t)77, engine::platform::retrieve("_non_existing"_u, (int64_t)77));
	EXPECT_EQ((uint8_t)77, engine::platform::retrieve("_non_existing"_u, (uint8_t)77));
	EXPECT_EQ((uint16_t)77, engine::platform::retrieve("_non_existing"_u, (uint16_t)77));
	EXPECT_EQ((uint32_t)77, engine::platform::retrieve("_non_existing"_u, (uint32_t)77));
	EXPECT_EQ((uint64_t)77, engine::platform::retrieve("_non_existing"_u, (uint64_t)77));
	EXPECT_EQ(true, engine::platform::retrieve("_non_existing"_u, true));
	EXPECT_EQ(false, engine::platform::retrieve("_non_existing"_u, false));
	EXPECT_EQ("foo_bar"_u, engine::platform::retrieve("_non_existing"_u, "foo_bar"_u));
}

TEST(platform_config_storage, invalid_keyname)
{
	EXPECT_EQ((int8_t)77, engine::platform::retrieve("invalid?*"_u, (int8_t)77));
	EXPECT_EQ((int16_t)77, engine::platform::retrieve("invalid?*"_u, (int16_t)77));
	EXPECT_EQ((int32_t)77, engine::platform::retrieve("invalid?*"_u, (int32_t)77));
	EXPECT_EQ((int64_t)77, engine::platform::retrieve("invalid?*"_u, (int64_t)77));
	EXPECT_EQ((uint8_t)77, engine::platform::retrieve("invalid?*"_u, (uint8_t)77));
	EXPECT_EQ((uint16_t)77, engine::platform::retrieve("invalid?*"_u, (uint16_t)77));
	EXPECT_EQ((uint32_t)77, engine::platform::retrieve("invalid?*"_u, (uint32_t)77));
	EXPECT_EQ((uint64_t)77, engine::platform::retrieve("invalid?*"_u, (uint64_t)77));
	EXPECT_EQ(true, engine::platform::retrieve("invalid?*"_u, true));
	EXPECT_EQ(false, engine::platform::retrieve("invalid?*"_u, false));
	EXPECT_EQ("foo_bar"_u, engine::platform::retrieve("invalid?*"_u, "foo_bar"_u));
}

TEST(platform_config_storage, unicode)
{
	TEST_STORE_AND_RETRIEVE("test", u8"zażółć gęślą jaźń"_u, "bar_foo"_u);
	TEST_STORE_AND_RETRIEVE("test", "foo_bar"_u, u8"zażółć gęślą jaźń"_u);
	TEST_STORE_AND_RETRIEVE("test", u8"こんにちは世界"_u, ""_u);
	TEST_STORE_AND_RETRIEVE("test", ""_u, u8"こんにちは世界"_u);
	TEST_STORE_AND_RETRIEVE("test", u8"♠♥♦♣♤♡♢♧"_u, ""_u);
	TEST_STORE_AND_RETRIEVE("test", ""_u, u8"♠♥♦♣♤♡♢♧"_u);
	TEST_STORE_AND_RETRIEVE("test", u8"\x1\x2\x3\x4"_u, ""_u);
	TEST_STORE_AND_RETRIEVE("test", ""_u, u8"\x1\x2\x3\x4"_u);
	TEST_STORE_AND_RETRIEVE("test", "\t\t"_u, ""_u);
	TEST_STORE_AND_RETRIEVE("test", "\ntest\n"_u, ""_u);
}
