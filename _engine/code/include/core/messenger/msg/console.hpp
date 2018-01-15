#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONSOLE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONSOLE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"

namespace engine
{
	
    namespace messenger
    {

		class msg_console_t
		{

		public:

			static constexpr bool is_queue_async = true;
			static constexpr bool is_instance_async = true;
			static constexpr bool keep_history = true;

			class actual_t
			{

			public:

#include "def/enum/console_msg.def"

				virtual ~actual_t();
		
				type_t get_type() const;
				source_t get_source() const;
				kind_t get_kind() const;
				
			protected:
		
				actual_t(type_t type, source_t source, kind_t kind);
				
			private:
		
				type_t type;
				source_t source;
				kind_t kind;
			};

			msg_console_t(std::unique_ptr<actual_t> actual);

			actual_t * get();
			

		private:

			std::unique_ptr<actual_t> actual;

		};

	}

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/console_msg.def"
#include "core/utility/enum_to_string.hpp"

#endif