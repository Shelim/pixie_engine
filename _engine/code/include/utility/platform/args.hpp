#ifndef ENGINE_COMMON_UTILITY_PLATFORM_ARGS_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_ARGS_HPP
#pragma once

#include<memory>
#include "utility/text/ustring.hpp"

namespace engine
{

    class args_t
    {

    public:

        args_t(int argc, char * argv[])
        {
            for(int i = 0; i < argc; i++)
            {
                data.push_back(ustring_t::from_utf8(argv[i]));
            }
        }

        int get_argc() const
        {
            return data.size();
        }

        const char * get_argv(int i) const
        {
            return data[i].get_cstring();
        }

        std::vector<const char*> get_argv() const
        {
            std::vector<const char *> ret;
            ret.resize(data.size());

            for(int i = 0; i < data.size(); i++)
            {
                ret[i] = data[i].get_cstring();
            }

            return ret;
        }

    private:

        ustring_collection_t data;

    };

}

#endif