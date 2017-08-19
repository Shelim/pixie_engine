/// USING Google Test engine

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"

namespace engine
{
	inline void PrintTo(const ustring_t& str, ::std::ostream* os)
	{
		*os << str.to_utf8();
	}
	inline void PrintTo(const virtual_path_t& path, ::std::ostream* os)
	{
		engine::ustring_t str = engine::to_string(path);
		*os << str.get_cstring();
	}
}