#include "utility/data/input.hpp"

std::unique_ptr<engine::data::input_t> engine::data::input_t::spawn_partial(int32_t size)
{
	buffer_t buff;
	buff.resize(size);
	read(&buff[0], size);

	return std::move(std::make_unique<input_partial_t>(path, std::move(buff)));
}