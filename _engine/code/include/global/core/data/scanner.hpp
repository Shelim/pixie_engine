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

			scanner_t(scanner_t const&) = delete;
			scanner_t(scanner_t &&) = default;
			scanner_t& operator=(scanner_t const&) = delete;
			scanner_t& operator=(scanner_t &&) = default;

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
			
			friend class scanners_t;

			virtual void scan_local(callback_t on_found)
			{

			}

		};
	}

}

#include "global/core/data/scanner/directory.hpp"

#endif