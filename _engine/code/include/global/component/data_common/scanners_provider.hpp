
#ifndef ENGINE_COMPONENT_DATA_COMMON_SCANNERS_PROVIDER_HPP
#define ENGINE_COMPONENT_DATA_COMMON_SCANNERS_PROVIDER_HPP
#pragma once

#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "utility/pattern/class_settings.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/core/data/scanner.hpp"

namespace engine
{

	class data_scanners_provider_base_t
	{

	public:

		virtual ~data_scanners_provider_base_t()
		{

		}

        virtual void add_scanners(data::scanners_t & scanners) = 0;

	};

	class data_scanners_t
	{
		
	};

	REGISTER_PROVIDER_BASE_TYPE(data_scanners_t, data_scanners_provider_base_t)

}

#include "global/component/data_common/scanners_provider/filesystem.hpp"
#include "global/component/data_common/scanners_provider/mockup.hpp"

#endif