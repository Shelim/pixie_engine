#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNER_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNER_HPP
#pragma once

#include "core/vfs/virtual_path.hpp"
#include "core/data/provider_actual.hpp"
#include <map>

namespace engine
{
	namespace data
	{
		class results_t;

		class scanner_t
		{

		public:

			virtual ~scanner_t()
			{

			}

			scanner_t(scanner_t const&) = delete;
			scanner_t(scanner_t &&) = default;
			scanner_t& operator=(scanner_t const&) = delete;
			scanner_t& operator=(scanner_t &&) = default;

			void scan(results_t  * results)
			{
				scan_local(results);
			}

		protected:

			scanner_t()
			{

			}
			
		private:
			
			friend class scanners_t;

			virtual void scan_local(results_t  * results)
			{

			}

		};
	}

}

#include "core/data/scanner/directory.hpp"

#endif