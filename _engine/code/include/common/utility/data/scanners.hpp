#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNERS_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNERS_HPP
#pragma once

#include "utility/data/scanner.hpp"
#include "utility/data/provider.hpp"
#include <vector>
#include <memory>

namespace engine
{
	namespace data
	{
		class state_t;

		class scanners_t final
		{

		public:

			typedef std::vector<std::unique_ptr<scanner_t> >  collection_t;

			scanners_t(std::unique_ptr<collection_t> scanners) : scanners(std::move(scanners))
			{

			}

			void scan(state_t * results);

		private:

			void add_results(state_t * results, scanner_t * scanner);

			std::unique_ptr<collection_t> scanners;

		};
	}

}

#endif