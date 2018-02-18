#ifndef ENGINE_COMPONENT_LOGGER_PROVIDER_TEMP_FILE_HPP
#define ENGINE_COMPONENT_LOGGER_PROVIDER_TEMP_FILE_HPP
#pragma once

#include "global/component/logger/real.hpp"

#include <fstream>

namespace engine
{
    class logger_provider_temp_file_t : public logger_provider_base_t
	{

	public:

        logger_provider_temp_file_t() : fout("C:/pixie.txt")
        {
            fout << "#####################\n### LOGGER OUTPUT ###\n#####################\n";
        }

        ~logger_provider_temp_file_t()
        {
            fout << "\n##################\n### CLEAN EXIT ###\n##################\n";
        }

		void output(const engine::console::logger_item_t & item) const final
        {
            ustring_t format;
            switch(item.get_kind())
            {
                case console::logger_item_t::kind_t::meta:
                case console::logger_item_t::kind_t::info:
                case console::logger_item_t::kind_t::msg:
                    format = " msg ###1:id# <#2:module#> #3:message#   time: #8:time#, thread: #9:thread#, func: #4:function#()   at: #5:file#, on line: #6:line#"_u;
                break;
                case console::logger_item_t::kind_t::pending:
                    format = " msg ###1:id# <#2:module#> #3:message#...   time: #8:time#, thread: #9:thread#, func: #4:function#()   at: #5:file#, on line: #6:line#"_u;
                break;
                case console::logger_item_t::kind_t::success:
                    format = " msg ###1:id# <#2:module#> Completed ###10:link#: '#3:message#'   time: #8:time#, thread: #9:thread#, func: #4:function#()   at: #5:file#, on line: #6:line#"_u;
                break;
                case console::logger_item_t::kind_t::failure:
                    format = "ERR  ###1:id# <#2:module#> FAILED ###10:link#: '#3:message#'   time: #8:time#, thread: #9:thread#, func: #4:function#()   at: #5:file#, on line: #6:line#"_u;
                break;
                case console::logger_item_t::kind_t::warning:
                    format = "WRN  ###1:id# <#2:module#> #3:message#   time: #8:time#, thread: #9:thread#, func: #4:function#()   at: #5:file#, on line: #6:line#"_u;
                break;
                case console::logger_item_t::kind_t::error:
                    format = "ERR  ###1:id# <#2:module#> #3:message#   time: #8:time#, thread: #9:thread#, func: #4:function#()   at: #5:file#, on line: #6:line#"_u;
                break;
            }

            ustring_t output = format_string(format, item.get_id(), item.get_source(), item.get_message(), item.get_function(), item.get_file(), item.get_line(), 0, item.get_time(), item.get_thread(), item.get_link());
            fout << output.get_cstring() << std::endl;
        }

    private:

        mutable std::ofstream fout;

    };


}

#endif