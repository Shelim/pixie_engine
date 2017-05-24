#ifndef ENGINE_COMMON_DATA_SCANNING_RESULT_HPP
#define ENGINE_COMMON_DATA_SCANNING_RESULT_HPP
#pragma once

#include "common/data/scanner.hpp"
#include "common/data/provider.hpp"
#include "common/data/database_state.hpp"
#include <vector>
#include <memory>

namespace engine
{
	namespace data
	{
		class scanners_t final
		{

		public:

			typedef std::vector<std::unique_ptr<scanner_t> >  collection_t;

			scanners_t(std::unique_ptr<collection_t> scanners) : scanners(std::move(scanners))
			{

			}

			void scan(database_state_t * results)
			{
				if (!results) return;

				results->clear();
				for (auto & scanner : *scanners)
				{
					scanner->scan();
					add_results(results, scanner.get());
				}
				results->calculate_directories();
			}

		private:

			void add_results(database_state_t * results, scanner_t * scanner)
			{
				for (auto & result : scanner->results)
				{
					results->add_provider_actual(std::move(result.second));
				}
			}

			std::unique_ptr<collection_t> scanners;

		};
	}

}

#endif