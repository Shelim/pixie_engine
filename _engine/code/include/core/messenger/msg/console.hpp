#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONSOLE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONSOLE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "component/config.hpp"

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

				enum kind_t
				{
#define ENGINE_CONSOLE_OUTPUT_KIND_STD(kind) kind,
#include "std/console_std.hpp"
					count
				};
			
				enum class source_t
				{
					unknown,
#define ENGINE_CONSOLE_SOURCE_STD(source) source,
#include "std/console_std.hpp"
					count
				};

				enum class type_t
				{
#define ENGINE_CONSOLE_MSG_TYPE_STD(type) type,
#include "std/console_std.hpp"
					count
				};

				static ustring_t get_kind_name(kind_t val)
				{
#define ENGINE_CONSOLE_OUTPUT_KIND_STD(kind) if(val == kind_t::kind) return #kind##_u;
#include "std/console_std.hpp"
					return "unknown"_u;
				}

				static ustring_t get_source_name(source_t val)
				{
#define ENGINE_CONSOLE_SOURCE_STD(source) if(val == source_t::source) return #source##_u;
#include "std/console_std.hpp"
					return "unknown"_u;
				}

				static ustring_t get_type_name(type_t val)
				{
#define ENGINE_CONSOLE_MSG_TYPE_STD(type) if(val == type_t::type) return #type##_u;
#include "std/console_std.hpp"
					return "unknown"_u;
				}

				virtual ~actual_t()
				{
		
				}
		
				type_t get_type() const
				{
					return type;
				}
		
				source_t get_source() const
				{
					return source;
				}
		
				kind_t get_kind() const
				{
					return kind;
				}
				
			protected:
		
				actual_t(type_t type, source_t source, kind_t kind) : type(type), source(source), kind(kind)
				{
		
				}
				
			private:
		
				type_t type;
				source_t source;
				kind_t kind;
			};

			msg_console_t(std::unique_ptr<actual_t> actual) : actual(std::move(actual))
			{

			}

			actual_t * get()
			{
				return actual.get();
			}
			

		private:

			std::unique_ptr<actual_t> actual;

		};

	}
}

#endif