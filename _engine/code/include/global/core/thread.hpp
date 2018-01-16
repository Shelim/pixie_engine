#ifndef ENGINE_GLOBAL_CORE_THREAD_HPP
#define ENGINE_GLOBAL_CORE_THREAD_HPP
#pragma once

#include "utility/pattern/fourcc.hpp"

namespace engine
{

    namespace global
    {

        class core_thread_t // For example: renderer thread!
        {

            public:

                core_thread_t(id_t id, ustring_t name) : id(id), name(name)
                {

                }

                id_t get_id() const
                {
                    return id;
                }

                const ustring_t & get_name() const
                {
                    return name;
                }

                class instance_t
                {

                    public:

                        ~instance_t()
                        {

                        }

                    private:

                        instance_t(core_thread_t * owner) : owner(owner)
                        {

                        }

                        core_thread_t * owner;

                        friend class core_thread_t;

                };

                typedef std::function<void()> callback_t;

                std::unique_ptr<instance_t> register_callback(callback_t callback);

                void execute_thread_once();

                ~core_thread_t()
                {
                    
                }

            private:

                id_t id;
                ustring_t name;

        };
    }

}

#endif