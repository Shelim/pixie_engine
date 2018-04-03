#ifndef ENGINE_GLOBAL_CORE_PROGRAM_HPP
#define ENGINE_GLOBAL_CORE_PROGRAM_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>
#include "global/core/program/reference.hpp"
#include "global/component/app_accounter.hpp"
#include "global/component/app_interrupter.hpp"
#include "global/component/app_overseer.hpp"
#include "global/component/app_resolver.hpp"
#include "global/component/config.hpp"
#include "global/component/config_monitor.hpp"
#include "global/component/config_storage.hpp"
#include "global/component/environment_info.hpp"
#include "global/component/extinguisher.hpp"
#include "global/component/filesystem.hpp"
#include "global/component/logger.hpp"
#include "global/component/program_args.hpp"
#include "global/component/program_instancer.hpp"
#include "global/component/program_instances_communicator_client.hpp"
#include "global/component/program_instances_communicator_host.hpp"
#include "global/component/program_instances_detection.hpp"
#include "global/component/thread_accounter.hpp"
#include "global/component/thread_program.hpp"

namespace engine
{

    class program_t
    {

    public:

        virtual ~program_t()
        {

        }

        std::shared_ptr<program_reference_t> get_program_reference() { return program_reference; }
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<component##_t> get_##component() { return component; }
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"

    protected:

        program_t()
        {

        }

        std::shared_ptr<program_reference_t> program_reference;
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<component##_t> component;
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"


    };

    template<class injector_t> class program_real_t : public program_t
    {

    public:

        program_real_t(injector_t injector)
        {
            this->program_reference = std::make_shared<program_reference_t>(this);
            auto injector_with_reference = boost::di::make_injector(std::move(injector), boost::di::bind<>().to(program_reference));
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) this->component = injector_with_reference.create<std::shared_ptr<component##_t> >();
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
        }

    };

    template<class injector_t> std::shared_ptr<program_t> make_program(injector_t injector)
    {
        return std::make_shared<program_real_t<injector_t> >(std::move(injector));
    }
    

}

#endif