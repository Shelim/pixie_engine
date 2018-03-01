#ifndef ENGINE_GLOBAL_CORE_TEST_RUNNER_HPP
#define ENGINE_GLOBAL_CORE_TEST_RUNNER_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>
#include <gtest/gtest.h>
#include "utility/concurrention/signal.hpp"
#include "utility/platform/args.hpp"
#include "utility/pattern/flags.hpp"

namespace engine
{

    class test_runner_t
    {

    public:

        test_runner_t() : return_code(1)
        {

        }

        ~test_runner_t()
        {
            if(worker.joinable())
                worker.join();
        }

        void run_test(int argc, char * argv[])
        {
            args = std::make_unique<args_t>(argc, argv);

            run_test_actual();
        }
        bool has_completed()
        {
            return signal.is_released();
        }
        void wait_till_completed()
        {
            signal.wait();
        }
        int32_t get_return_code()
        { 
            return return_code; 
        }

    private:

        std::unique_ptr<args_t> args;
        int32_t return_code;
        signal_t signal;
        std::thread worker;

        void run_test_actual()
        {
            worker = std::thread([this]{

            int argc = args->get_argc();
            std::vector<const char*> argv = args->get_argv();

            testing::InitGoogleTest(&argc, const_cast<char**>(&argv[0]));
            return_code = RUN_ALL_TESTS();
            signal.signal();
            
            });
        }

    };
}


#endif