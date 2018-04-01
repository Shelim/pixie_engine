#ifndef ENGINE_COMMON_UTILITY_PLATFORM_ARGS_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_ARGS_HPP
#pragma once

#include<memory>
#include <map>
#include "utility/text/ustring.hpp"

namespace engine
{

    class args_t
    {

    public:

        args_t(int argc, const char * argv[]): raw(argc, argv)
        {
            parsed = parse_args(raw);
            raw = parsed.construct_raw();
        }
        args_t(const char * args): raw(args)
        {
            parsed = parse_args(raw);
            raw = parsed.construct_raw();
        }
        args_t(const args_t & other) : parsed(other.parsed), raw(other.raw)
        {

        }
        args_t(args_t && other) : parsed(std::move(other.parsed)), raw(std::move(other.raw))
        {

        }

        args_t & operator=(const args_t & other)
        {
            parsed = other.parsed;
            raw = other.raw;
            return (*this);
        }

        int get_raw_argc() const
        {
            return raw.get_raw_argc();
        }

        const char * get_raw_argv(int i) const
        {
            return raw.get_raw_argv(i);
        }

        int get_argc() const
        {
            return raw.get_argc();
        }

        ustring_t get_argv(int i) const
        {
            return raw.get_argv(i);
        }

        std::vector<const char*> get_raw_argv() const
        {
            return raw.get_raw_argv();
        }

        ustring_t get_args() const
        {
            return raw.get_args();
        }

        ustring_t get_executable() const
        {
            return parsed.get_executable();
        }

        void set_executable(ustring_t value)
        {
            parsed.set_executable(value);
            raw = parsed.construct_raw();
        }

        ustring_t get_arg(ustring_t key, ustring_t def = ""_u) const
        {
            return parsed.get_arg(key, def);
        }

        void set_arg(ustring_t key, ustring_t value)
        {
            parsed.set_arg(key, value);
            raw = parsed.construct_raw();
        }

    private:

        class raw_t
        {
            public:

                raw_t(int argc, const char * argv[])
                {
                    set(argc, argv);
                }
                raw_t(const char * args)
                {
                    set(args);
                }
                raw_t(ustring_t args)
                {
                    set(args);
                }


                void set(int argc, const char * argv[])
                {
                    args_array.reserve(argc);

                    for(int i = 0; i < argc; i++)
                    {
                        if(!args_merged.is_empty()) args_merged.append(" "_u);
                        args_array.push_back(ustring_t::from_utf8(argv[i]));
                        args_merged.append(args_array[i]);
                    }
                }

                void set(ustring_t args)
                {
                    args_merged = args;

                    int index = args_merged.index_of(' ');
                    int prev_index = 0;
                    while(index != -1)
                    {
                        ustring_t arg = args_merged.substr(prev_index, index - prev_index);

                        if(!arg.is_empty())
                            args_array.push_back(arg);

                        prev_index = index;
                        index = args_merged.index_of(' ', index + 1);
                    }
                }
                void set(const char * args)
                {
                    set(ustring_t::from_utf8(args));
                }

                int get_raw_argc() const
                {
                    return args_array.size();
                }

                const char * get_raw_argv(int i) const
                {
                    return args_array[i].get_cstring();
                }

                std::vector<const char*> get_raw_argv() const
                {
                    std::vector<const char *> ret;
                    ret.resize(args_array.size());

                    for(int i = 0; i < args_array.size(); i++)
                    {
                        ret[i] = args_array[i].get_cstring();
                    }

                    return ret;
                }

                std::size_t get_argc() const
                {
                    return args_array.size();
                }

                ustring_t get_argv(int i) const
                {
                    return args_array[i];
                }

                ustring_t get_args() const
                {
                    return args_merged;
                }

            private:

                ustring_t args_merged;
                ustring_collection_t args_array;
        };

        class nice_t
        {

            public:

                ustring_t get_executable() const
                {
                    return executable;
                }

                void set_executable(ustring_t value)
                {
                    executable = value;
                }

                ustring_t get_arg(ustring_t key, ustring_t def = ""_u) const
                {
                    auto iter = args.find(key);
                    if(iter == args.end())
                        return def;
                    return iter->second;
                }

                void set_arg(ustring_t key, ustring_t value)
                {
                    args[key] = value;
                }

                raw_t construct_raw() const
                {
                    ustring_t builder = executable;
                    for(auto iter : args)
                    {
                        builder.append(" "_u);
                        if(iter.first.index_of(' ') != -1)
                        {
                            builder.append('"');
                            builder.append(iter.first);
                            builder.append('"');
                        }
                        else
                            builder.append(iter.first);
                        builder.append('=');
                        if(iter.second.index_of(' ') != -1)
                        {
                            builder.append('"');
                            builder.append(iter.second);
                            builder.append('"');
                        }
                        else
                            builder.append(iter.second);
                    }
                    return raw_t(builder);
                }

            private:
                
                ustring_t executable;
                std::map<ustring_t, ustring_t> args;
            
        };

        raw_t raw;
        nice_t parsed;

        static nice_t parse_args(const raw_t & args)
        {
            nice_t ret;
            ret.set_executable(args.get_argv(0));
            return ret;
        }

    };

}

#endif