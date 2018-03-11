#include <gtest/gtest.h>
#include "global/core/program/igniter.hpp"

TEST(GlobalExample, Sample)
{
    BEGIN_PLATFORM_CONFIGURATION(test)
    USE_STANDARD_POLICIES()
    ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(disabled)
    ENABLE_GLOBAL_COMPONENT(config_storage)
    USE_STANDARD_GLOBAL_PROVIDERS()
    USE_STANDARD_GLOBAL_SETTINGS()
    END_PLATFORM_CONFIGURATION()

    std::shared_ptr<engine::program_t> program = test.ignite_for_tests();
    
    ASSERT_TRUE(program);

    program->get_config_storage()->store("test"_u, true);
    EXPECT_TRUE(program->get_config_storage()->retrieve("test"_u, false));

}