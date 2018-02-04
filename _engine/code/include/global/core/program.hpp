#ifndef ENGINE_GLOBAL_CORE_PROGRAM_HPP
#define ENGINE_GLOBAL_CORE_PROGRAM_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>
#include "global/component/app_instancer.hpp"
#include "global/component/app_overseer.hpp"
#include "global/component/app_resolver.hpp"
#include "global/component/config.hpp"
#include "global/component/config_monitor.hpp"
#include "global/component/config_storage.hpp"
#include "global/component/debug_symbols.hpp"
#include "global/component/environment_info.hpp"
#include "global/component/extinguisher.hpp"
#include "global/component/filesystem.hpp"
#include "global/component/logger.hpp"
#include "global/component/program_args.hpp"
#include "global/component/program_instances_communicator_client.hpp"
#include "global/component/program_instances_communicator_host.hpp"
#include "global/component/program_instances_detection.hpp"
#include "global/component/thread_overseer.hpp"
#include "global/component/thread_program.hpp"

namespace engine
{

    class program_t
    {

    private:

        class unused_t {};

        friend class igniter_t;

    public:

        program_t(boost::di::injector<
#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t>,
#include "def/global_component.def"
        unused_t> injector)
        {
#define ENGINE_GLOBAL_COMPONENT_DEF(component) this->component = injector.create<std::shared_ptr<component##_t> >();
#include "def/global_component.def"
        }

#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t> get_##component() { return component; }
#include "def/global_component.def"

    private:

#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t> component;
#include "def/global_component.def"


    };

}

#endif