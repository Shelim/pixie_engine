#ifndef ENGINE_COMMON_UTILITY_PATTERN_WRITER_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_WRITER_HPP
#pragma once

#include <memory>
#include <vector>

namespace engine
{

	/**
	 * @brief Generic class for writing messages into stream (for example @ref core_console "console")
	 * 
	 * This class is overriden by various streaming API for writing
	 * 
	 * @tparam msg_type_t type of messages to be written 
	 */
	template<class msg_type_t> class writer_base_t
	{

	public:

		virtual ~writer_base_t()
		{

		}

		/**
		 * @brief Class for writing several messages in strong order without interruptions
		 * 
		 * This class allows uninterrupted flow of messages.
		 * This is very useful in concurrent environment when there are more than one writers for given stream
		 */
		class transaction_t
		{

		public:

			~transaction_t()
			{

			}

			/**
			 * @brief Prepares given message for writing
			 * 
			 * Prepares message for writing. Entire writing will be conducted at transaction commit phase (typically end function of @ref writer_base_t )
			 * 
			 * @param[in] msg Message to be written
			 */
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

		/**
		 * @brief Start new transaction.
		 * 
		 * Transaction gives strong guarantee for order and non-interrupted flow of messages for this stream.
		 * This is very useful in concurrent environment when there are more than one writers for given stream
		 *
		 * @note There can be as many transactions available at once as desired
		 * 
		 * Transaction is written at commit phase, typically when called end()
		 * 
		 * @return transaction object
		 * @see transaction_t, end()
		 */
		std::unique_ptr<transaction_t> begin()
		{
			return std::make_unique<transaction_t>();
		}

		/**
		 * @brief Execute transaction
		 * 
		 * Write all messages stored in transaction to this stream in order and non-interrupted flow.
		 * This is very useful in concurrent environment when there are more than one writers for given stream
		 * 
		 * @param[in] transaction To be written
		 */
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

		/**
		 * @brief Writes message to this stream
		 * 
		 * @param[in] msg Message to be written
		 */
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