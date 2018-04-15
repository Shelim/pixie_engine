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

		protected:

			scanner_t()
			{

			}
			
		private:

			virtual void scan_local(callback_t on_found)
			{

			}

		};

		typedef std::vector<std::unique_ptr<scanner_t> > scanners_t;
	}

}

#include "global/core/data/scanner/archive.hpp"
#include "global/core/data/scanner/directory.hpp"

#endif