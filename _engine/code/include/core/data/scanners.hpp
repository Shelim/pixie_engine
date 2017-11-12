#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNERS_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNERS_HPP
#pragma once

#include "core/data/scanner.hpp"
#include "core/data/provider.hpp"
#include <vector>
#include <memory>

namespace engine
{
	namespace data
	{
		class results_t;

		class scanners_t final
		{

		public:

			typedef std::vector<std::unique_ptr<scanner_t> >  collection_t;

			scanners_t(std::unique_ptr<collection_t> scanners) : scanners(std::move(scanners))
			{

			}

			void scan(results_t * results);

		private:

			std::unique_ptr<collection_t> scanners;

		};
	}

}

#endif