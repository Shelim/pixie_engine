#include <gtest/gtest.h>
#include "global/core/program/igniter.hpp"
#include "utility/platform/args.hpp"

TEST(LocalExample, Sample)
{
    BEGIN_PLATFORM_CONFIGURATION(test)
    USE_STANDARD_POLICIES()
    ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(disabled)
    USE_STANDARD_GLOBAL_PROVIDERS()
    USE_STANDARD_GLOBAL_SETTINGS()
    END_PLATFORM_CONFIGURATION()

    std::shared_ptr<engine::program_t> program = test.ignite_for_tests();
    
    ASSERT_TRUE(program);

    std::unique_ptr<engine::app_context_t> context = std::make_unique<engine::app_context_t>(program, engine::args_t(""));

    BEGIN_BOOTSTRAPPER(test_app, context.get())
    ALL_COMPONENTS_BY_DEFAULT_ARE(disabled)
    ENABLE_COMPONENT(terminal)
    USE_STANDARD_LOCAL_PROVIDERS()
    USE_STANDARD_LOCAL_SETTINGS()
    APP(game)
    END_BOOTSTRAPPER()

    auto terminal = test_app.get_terminal();

    ASSERT_TRUE(terminal);

}