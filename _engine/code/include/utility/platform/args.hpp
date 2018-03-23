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
            arg_collection_raw.reserve(argc);
            ustring_t args_full;

            for(int i = 0; i < argc; i++)
            {
                if(i > 0) args_full.append(" "_u);
                arg_collection_raw.push_back(ustring_t::from_utf8(argv[i]));
                args_full.append(arg_collection_raw[i]);
            }

            parse_args(args_full);
        }
        args_t(char * args)
        {
            ustring_t args_full = ustring_t::from_utf8(args);

            int index = args_full.index_of(' ');
            int prev_index = 0;
            while(index != -1)
            {
                ustring_t arg = args_full.substr(prev_index, index - prev_index);

                if(!arg.is_empty())
                    arg_collection_raw.push_back(arg);

                prev_index = index;
                index = args_full.index_of(' ', index + 1);
            }

            parse_args(args_full);
        }

        int get_raw_argc() const
        {
            return arg_collection_raw.size();
        }

        const char * get_raw_argv(int i) const
        {
            return arg_collection_raw[i].get_cstring();
        }

        std::vector<const char*> get_raw_argv() const
        {
            std::vector<const char *> ret;
            ret.resize(arg_collection_raw.size());

            for(int i = 0; i < arg_collection_raw.size(); i++)
            {
                ret[i] = arg_collection_raw[i].get_cstring();
            }

            return ret;
        }

        ustring_t get_cmd_line() const
        {
            ustring_t ret;

            for(int i = 0; i < arg_collection_raw.size(); i++)
            {
                if(i > 0) ret.append(" "_u);
                ret.append(arg_collection_raw[i]);
            }

            return ret;
        }

    private:

        ustring_collection_t arg_collection_raw;

        void parse_args(const ustring_t & args)
        {
            
        }

    };

}

#endif