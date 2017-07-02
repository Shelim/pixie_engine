#ifndef ENGINE_COMMON_UTILITY_PATTERN_FACTORY_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FACTORY_HPP
#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace engine
{
	template<class key_t> class factorable_t;

	template<typename base_t, typename key_t> class factory_t final
	{
	public:
		static_assert(std::is_base_of<factorable_t<key_t>, base_t>::value, "Factory requires that base object is derived from factorable object");

		template<typename T> void register_type(const key_t & name)
		{
			static_assert(std::is_base_of<base_t, T>::value, "Factory::Register doesn't accept this type because doesn't derive from base class");
			creators[name] = &create_func<T>;
		}

		void unregister_type(const key_t & name)
		{
			creators.erase(name);
		}

		std::shared_ptr<base_t> create(const key_t & name)
		{
			auto func = creators.find(name);
			if (func == creators.end()) return nullptr;
			std::shared_ptr<base_t> ret = func->second(name);
			return ret;
		}

	private:

		template<typename T> static std::shared_ptr<base_t> create_func(const key & name)
		{
			std::shared_ptr<base_t> ret = std::make_shared<T>();
			set_key(ret, name);
			return ret;
		}

		static void set_key(std::shared_ptr<base_t> ret, const key_t & name)
		{
			std::static_pointer_cast<factorable_t<key_t>>(ret)->type = name;
		}

		typedef std::shared_ptr<base_t>(*creator)(const key_t & name);

		std::unordered_map<key_t, creator> creators;
	};

	template<class key_t> class factorable_t
	{
	public:
		virtual ~factorable_t() { }

		const key_t & get_type()
		{
			return type;
		}

		template <typename base_t, typename key2_t> friend void factory_t<base_t, key2_t>::set_key(std::shared_ptr<base_t> ret, const key2_t & name);

	private:
		key_t type;
	};

}

#endif
