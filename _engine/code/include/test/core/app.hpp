// USING Google Test engine
#ifndef ENGINE_TEST_APP_HPP
#define ENGINE_TEST_APP_HPP
#pragma once

#include <gtest/gtest.h>
#include "global/app.hpp"

namespace engine
{
	class app_tester_t : public app_t
    {
        public:

            return_code_t main(const ustring_collection_t & args) final
            {
                return return_code_t::success;
            }

            manifest_app_t::app_t get_app() final
            {
                return manifest_app_t::app_t::tester;
            }
            
    };
}

#endif