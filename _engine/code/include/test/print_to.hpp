/// USING Google Test engine

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "common/utility/text/ustring.hpp"

namespace engine
{
	inline void PrintTo(const ustring_t& str, ::std::ostream* os)
	{
		*os << str.to_utf8();
	}
}