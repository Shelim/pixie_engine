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

	template <class T>
	struct ifactory : ifactory_base {
		virtual ~ifactory() noexcept = default;
		virtual std::unique_ptr<T> create() const { return std::unique_ptr<T>(); }
	};

	template <class, class, class>
	struct factory_impl;

	template <class TInjector, class T, class I>
	struct factory_impl<TInjector, T, ifactory<I>> : ifactory<I> {
		explicit factory_impl(const TInjector& injector) : injector_(const_cast<TInjector&>(injector)) {}

		std::unique_ptr<I> create() const override {
			return injector_.template create<std::unique_ptr<T>>();
		}

	private:
		TInjector & injector_;
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
