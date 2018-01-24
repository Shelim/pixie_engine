#ifndef ENGINE_GLOBAL_COMPONENTS_HPP
#define ENGINE_GLOBAL_COMPONENTS_HPP
#pragma once

#include "global/component/app_resolver.hpp"
#include "global/component/app_runner.hpp"
#include "global/component/apps_running.hpp"
#include "global/component/global_thread.hpp"
#include "global/component/program_singleton.hpp"

#include <memory>

namespace engine
{

    namespace global
    {        

        class components_t
        {

        public:

            class unusued_t {};

            components_t(
#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t> component,
#include "def/global_component.def"
                std::unique_ptr<unusued_t> unused
            ) : 
#define ENGINE_GLOBAL_COMPONENT_DEF(component) component(component),
#include "def/global_component.def"
                unused(std::move(unused))
            {
                this->unused.reset();
            }

#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t> get_##component() { return component; }
#include "def/global_component.def"

        private:

#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t> component;
#include "def/global_component.def"
            std::unique_ptr<unusued_t> unused;

        };

    }

}

#endif