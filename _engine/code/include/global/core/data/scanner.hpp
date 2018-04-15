#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNER_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNER_HPP
#pragma once

#include "global/core/vfs/virtual_path.hpp"
#include "global/core/data/provider_actual.hpp"
#include <map>

namespace engine
{
	namespace data
	{

		class scanner_t
		{

		public:

			virtual ~scanner_t()
			{

			}

			typedef std::function<void(std::unique_ptr<provider_actual_t>)> callback_t;

			void scan(callback_t on_found)
			{
				scan_local(std::move(on_found));
			}

#define ENGINE_DATA_SCANNER_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(kind_t, __VA_ARGS__)
#include "def/data_scanner.def"

			kind_t get_kind() const
			{
				return kind;
			}

		protected:

			scanner_t(kind_t kind) : kind(kind)
			{

			}
			
		private:

			virtual void scan_local(callback_t on_found)
			{

			}
			kind_t kind;

		};

		typedef std::vector<std::unique_ptr<scanner_t> > scanners_t;
	}

}

#include "global/core/data/scanner/archive.hpp"
#include "global/core/data/scanner/directory.hpp"

namespace engine
{
	namespace data
	{
		bool operator==(const scanner_t & scanner1, const scanner_t & scanner2)
		{
			if(scanner1.get_kind() != scanner2.get_kind()) return false;
			
			switch(scanner1.get_kind())
			{
#define ENGINE_DATA_SCANNER_IMPL(name) case engine::data::scanner_t::kind_t::name: return static_cast<const engine::data::scanner_##name##_t &>(scanner1) == static_cast<const engine::data::scanner_##name##_t &>(scanner2);
#define ENGINE_DATA_SCANNER_DEF(...) DEFINE_TYPE_PASS(ENGINE_DATA_SCANNER_IMPL, __VA_ARGS__)
#include "def/data_scanner.def"
			}
			return false;
		}
	}
}


#endif