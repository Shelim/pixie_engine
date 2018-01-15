#ifndef ENGINE_COMMON_UTILITY_PATTERN_FACTORY_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FACTORY_HPP
#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include "boost/di.hpp"

namespace engine
{

	struct ifactory_base
	{
		virtual ~ifactory_base() noexcept = default;
	};

	/**
	 * @brief Abstract factory for given object T.
	 * 
	 * You can request factory for given object in constructor of any classes
	 * by using following code:
	 * 
	 * @code{.cpp}
	 * class example_t
	 * {
	 *   public:
	 *     example_t(std::shared_ptr<ifactory<requested>> factory)
	 *     {
	 *       std::unique_ptr<requested> obj = factory.create();
	 *     }
	 * };
	 * @endcode
	 * 
	 * @note This class is bound by @ref bootstrapper to concrete factory.
	 * 
	 * @warning To make object factorable you will need to mark it inside @c def/factorable.def file
	 * 
	 * You can use this factory for assisted creation (ie. when you need to pass a few custom arguments besides default ones):
	 * 
	 * @code{.cpp}
	 * class example2_t
	 * {
	 *    public:
	 *      example2_t(std::shared_ptr<ifactory<requested, int, float>> factory)
	 *      {
	 * 	      // requested constructor will recieve 5 and 7.5f for their non-injected arguments
	 *         std::unique_ptr<requested> obj = factory.create(5, 7.5f);
	 * 	    }
	 * };
	 * @endcode
	 * 
	 * @tparam T Object to construct (can be interface)
	 * @tparam TArgs Optional non-injectable by default arguments for constructor
	 */
	template <class T, class... TArgs>
	struct ifactory : ifactory_base {
		virtual ~ifactory() noexcept = default;
		/**
		 * @brief Creates new instance of T by using injected arguments
		 * 
		 * @param[in] TArgs Optional non-injectable by default arguments for constructor
		 * @return Constructed object
		 * @see ifactory
		 */
		virtual std::unique_ptr<T> create(TArgs&&...) const = 0;
	};

	template <class, class, class>
	struct factory_impl;

	template <class TInjector, class T, class I, class... TArgs>
	struct factory_impl<TInjector, T, ifactory<I, TArgs...>> : ifactory<I, TArgs...> {
		explicit factory_impl(const TInjector& injector) : injector_(const_cast<TInjector&>(injector)) {}

		std::unique_ptr<I> create(TArgs&&... args) const override {
			// clang-format off
			auto injector = boost::di::make_injector(
				std::move(injector_)
	#if (__clang_major__ == 3) && (__clang_minor__ > 4) || defined(__GCC___) || defined(__MSVC__)
				, boost::di::bind<TArgs>().to(std::forward<TArgs>(args))[boost::di::override]...
	#else // wknd for clang 3.4
				, boost::di::core::dependency<boost::di::scopes::instance, TArgs, TArgs, boost::di::no_name, boost::di::core::override>(std::forward<TArgs>(args))...
	#endif
			);
			// clang-format on

			std::unique_ptr<T> object;
	#ifdef __EXCEPTIONS
			try {
				object = injector.template create<std::unique_ptr<T>>();
			}
			catch (...) {
				injector_ = std::move(injector);
				throw;
			}
	#else
			object = injector.template create<std::unique_ptr<T>>();
	#endif
			injector_ = std::move(injector);
			return std::move(object);
		}

	private:
		TInjector& injector_;
	};

	template <class T>
	struct factory {
		template <class TInjector, class TDependency>
		auto operator()(const TInjector& injector, const TDependency&) noexcept {
			auto iter = data_map.find(std::type_index(typeid(typename factory<T>::key_t<TInjector, TDependency>)));
			std::shared_ptr<ifactory_base> ret;
			if (iter == data_map.end())
			{
				ret = std::make_shared<factory_impl<TInjector, T, typename TDependency::expected>>(injector);
				data_map.insert(std::make_pair(std::type_index(typeid(typename factory<T>::key_t<TInjector, TDependency>)), ret));
			}
			else
				ret = iter->second;
			return std::static_pointer_cast<factory_impl<TInjector, T, typename TDependency::expected>>(ret);
		}

	private:

		template <class T1, class T2> class key_t {};

		std::unordered_map<std::type_index, std::shared_ptr<ifactory_base> > data_map;

	};

}

#endif
