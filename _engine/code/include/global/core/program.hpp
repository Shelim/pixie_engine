#ifndef ENGINE_GLOBAL_CORE_PROGRAM_HPP
#define ENGINE_GLOBAL_CORE_PROGRAM_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>
#include "global/core/extinguisher.hpp"

namespace engine
{

    namespace global
    {

        class program_t
        {

        public:

            virtual ~program_t()
            {

            }

            virtual void wait_for_completion() = 0;

            virtual int32_t get_return_code() const = 0;

        private:

        };

    }

}

#define DEFINE_PROGRAM_HOLDER(name) std::unique_ptr<engine::global::program_t> name;

#include "global/core/program/real.hpp"

#endif