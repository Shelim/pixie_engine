#include "component/renderer_status.hpp"

engine::renderer_status_t::~renderer_status_t()
{

}

uint64_t engine::renderer_status_dummy_t::get_frame() const
{
    return 0;
}