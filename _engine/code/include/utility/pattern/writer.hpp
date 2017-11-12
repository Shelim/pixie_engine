#ifndef ENGINE_COMMON_UTILITY_PATTERN_WRITER_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_WRITER_HPP
#pragma once

#include <memory>
#include <vector>

namespace engine
{
	template<class msg_type_t> class writer_base_t
	{

	public:

		virtual ~writer_base_t()
		{

		}

		class transaction_t
		{

		public:

			~transaction_t()
			{

			}

			void write(std::unique_ptr<msg_type_t> msg)
			{

				std::lock_guard<std::mutex> guard(mutex);

				buffer.push_back(std::move(msg));
			}

		private:

			std::mutex mutex;
			std::vector<std::unique_ptr<msg_type_t> > buffer;

			transaction_t()
			{

			}

			friend class writer_base_t<msg_type_t>;

		};

		std::unique_ptr<transaction_t> begin()
		{
			return std::make_unique<transaction_t>();
		}

		void end(std::unique_ptr<transaction_t> transaction)
		{
			std::lock_guard<std::mutex> guard1(mutex);
			std::lock_guard<std::mutex> guard2(transaction->mutex);

			for (auto i = 0; i < transaction->buffer.size(); i++)
			{
				write_local(std::move(transaction->buffer[i]));
			}

			transaction->buffer.clear();
		}

		void write(std::unique_ptr<msg_type_t> msg)
		{
			std::lock_guard<std::mutex> guard(mutex);

			write_local(std::move(msg));
		}

	private:

		std::mutex mutex;

		virtual void write_local(std::unique_ptr<msg_type_t> msg) = 0;

	};


}

#endif