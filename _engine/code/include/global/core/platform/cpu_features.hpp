#ifndef ENGINE_COMMON_UTILITY_PLATFORM_CPU_FEATURES_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_CPU_FEATURES_HPP

#include "utility/text/ustring.hpp"
#include "utility/pattern/enum.hpp"
#include <bitset>

#pragma once

namespace engine
{

	class cpu_features_t
	{

	public:

        cpu_features_t()
        {
            
        }

#define ENGINE_CPU_FEATURE_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(feature_t, __VA_ARGS__)
#include "def/cpu_feature.def"

        bool is_feature(feature_t feature) const
        {
            return features.test(value_of(feature));
        }

	private:

        friend class builder_t;

        typedef std::bitset<value_of(feature_t::count)> features_t;

        cpu_features_t(const features_t & features) : features(features)
        {

        }

        features_t features;

    public:

        class builder_t
        {

            public:

                void set_feature(feature_t feature, bool enabled = true)
                {
                    features.set(value_of(feature), enabled);
                }

                cpu_features_t build()
                {
                    return cpu_features_t(features);
                }

            private:

                cpu_features_t::features_t features;

        };

	};

	template<> inline ustring_t to_string<cpu_features_t>(const cpu_features_t & item)
	{
		ustring_t ret;
#define ENGINE_CPU_FEATURE_IMPL(type, name)  if(item.is_feature(cpu_features_t::feature_t::type)) \
                                            { \
                                                if(!ret.is_empty()) ret.append(" "_u); \
                                                ret.append(name); \
                                            }
#define ENGINE_CPU_FEATURE_DEF(...) DEFINE_TYPE_PASS(ENGINE_CPU_FEATURE_IMPL, __VA_ARGS__)
#include "def/cpu_feature.def"
        return ret;
	}

	template<> inline cpu_features_t from_string<cpu_features_t>(const ustring_t & item)
	{

	}

}

#define ENGINE_CPU_FEATURE_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::cpu_features_t::feature_t, __VA_ARGS__)
#include "def/cpu_feature.def"

#endif