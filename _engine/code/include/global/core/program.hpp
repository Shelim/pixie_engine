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
#include "global/component/data_archive.hpp"
#include "global/component/data_common/filesystem_paths_provider.hpp"
#include "global/component/data_common/scanners_provider.hpp"
#include "global/component/data_creator.hpp"
#include "global/component/data_items.hpp"
#include "global/component/data_manipulator.hpp"
#include "global/component/data_monitor.hpp"
#include "global/component/data_state.hpp"
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

#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<component##_t> get_##component() { return component; }
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
#define ENGINE_BOOTSTRAPPER_IMPL(component) std::shared_ptr<component##_t> get_##component() { return component; }
#define ENGINE_BOOTSTRAPPER_DEF(...) DEFINE_TYPE_PASS(ENGINE_BOOTSTRAPPER_IMPL, __VA_ARGS__)
#include "def/bootstrapper.def"

    protected:

        program_t()
        {

        }

#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<component##_t> component;
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
#define ENGINE_BOOTSTRAPPER_IMPL(component) std::shared_ptr<component##_t> component;
#define ENGINE_BOOTSTRAPPER_DEF(...) DEFINE_TYPE_PASS(ENGINE_BOOTSTRAPPER_IMPL, __VA_ARGS__)
#include "def/bootstrapper.def"


    };

    template<class injector_t> class program_real_t : public program_t
    {

    public:

        program_real_t(injector_t injector)
        {
            auto injector_with_reference = boost::di::make_injector(std::move(injector), boost::di::bind<>().to(static_cast<program_t*>(this)));
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) this->component = injector_with_reference.create<std::shared_ptr<component##_t> >();
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
#define ENGINE_BOOTSTRAPPER_IMPL(component) this->component = injector_with_reference.create<std::shared_ptr<component##_t> >();
#define ENGINE_BOOTSTRAPPER_DEF(...) DEFINE_TYPE_PASS(ENGINE_BOOTSTRAPPER_IMPL, __VA_ARGS__)
#include "def/bootstrapper.def"
        }

    };

    template<class injector_t> std::shared_ptr<program_t> make_program(injector_t injector)
    {
        std::shared_ptr<program_t> ret = std::make_shared<program_real_t<injector_t> >(std::move(injector));
        
        std::unique_ptr<app_context_t> app_context = std::make_unique<app_context_t>(ret.get(), ret->get_program_args()->get_program_args());
        ret->get_app_overseer()->run_app(ret->get_app_resolver()->get_app_kind_for_given_context(app_context.get()), std::move(app_context));

        return ret;
    }
    

}

#endif