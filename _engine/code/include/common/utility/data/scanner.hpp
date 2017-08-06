#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNER_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNER_HPP
#pragma once

#include "utility/vfs/virtual_path.hpp"
#include "utility/data/provider_actual.hpp"
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

			scanner_t(scanner_t const&) = delete;
			scanner_t(scanner_t &&) = default;
			scanner_t& operator=(scanner_t const&) = delete;
			scanner_t& operator=(scanner_t &&) = default;

			typedef std::map<virtual_path_t, std::unique_ptr<provider_actual_t> > results_t;

			void scan()
			{
				results.clear();
				scan_local();
			}

		protected:

			void add_result(std::unique_ptr<provider_actual_t> result)
			{
				results[result->get_virtual_path()] = std::move(result);
			}

			scanner_t()
			{

			}
			
		private:
			
			friend class scanners_t;

			virtual void scan_local()
			{

			}

			results_t results;

		};
	}

}

#include "utility/data/scanner/directory.hpp"

#endif