#ifndef ENGINE_COMMON_UTILITY_RENDERER_COLOR_HPP
#define ENGINE_COMMON_UTILITY_RENDERER_COLOR_HPP
#pragma once

#include<cstdint>

namespace engine
{
	/**
	* \~English @brief Class for handling @c RGBA colors
	*
	* @note Order of bytes is ALWAYS @c RGBA 
	* @note Each channel is occupying exactly 1 byte
	*/
	class color_t final
	{

	public:

		/**
		* \~English @brief Creates new @c RGBA color
		*
		* @param[in] r Red component
		* @param[in] g Green component
		* @param[in] b Blue component
		* @param[in] a Alpha component (255 - fully opaque)
		*/
		color_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		{
			set_r(r);
			set_g(g);
			set_b(b);
			set_a(a);
		}

		/**
		* \~English @brief Creates new @c RGBA color
		*
		* @param[in] col Basic color. RGBA format
		*/
		color_t(uint32_t col)
		{
			set(col);
		}

		/**
		* \~English @brief Returns @c red component of color
		*
		* @return Red component
        * @see set_r, get, set
		*/
		uint8_t get_r() const
		{
			return data.r;
		}

		/**
		* \~English @brief Returns @c green component of color
		*
		* @return Green component
        * @see set_g, get, set
		*/
		uint8_t get_g() const
		{
			return data.g;
		}

		/**
		* \~English @brief Returns @c blue component of color
		*
		* @return Blue component
        * @see set_b, get, set
		*/
		uint8_t get_b() const
		{
			return data.b;
		}

		/**
		* \~English @brief Returns @c alpha component of color
		*
		* @note Value of 255 means fully opaque
		* @note While value 0 means fully transparent
		*
		* @return Alpha component
        * @see set_a, get, set
		*/
		uint8_t get_a() const
		{
			return data.a;
		}

		/**
		* \~English @brief Returns full representation of color.
		* \~English @brief Format is @c RGBA with each color taking 1 byte for the 4 bytes total
		*
		* @return Full color
        * @see set
		*/
		uint32_t get() const
		{
			return data.col;
		}

		/**
		* \~English @brief Sets new value for @c red channel
		*
		* @param[in] value New value for @c red channel
		*
		* @see get_r, get, set
		*/
		void set_r(uint8_t value)
		{
			data.r = value;
		}

		/**
		* \~English @brief Sets new value for @c green channel
		*
		* @param[in] value New value for @c green channel
		*
		* @see get_g, get, set
		*/
		void set_g(uint8_t value)
		{
			data.g = value;
		}

		/**
		* \~English @brief Sets new value for @c blue channel
		*
		* @param[in] value New value for @c blue channel
		*
		* @see get_b, get, set
		*/
		void set_b(uint8_t value)
		{
			data.b = value;
		}

		/**
		* \~English @brief Sets new value for @c alpha (opacity) channel
		*
		* @param[in] value New value for @c alpha channel
		*
		* @note Value of 255 means fully opaque
		* @note While value 0 means fully transparent
		*
		* @see get_b, get, set
		*/
		void set_a(uint8_t value)
		{
			data.a = value;
		}

		/**
		* \~English @brief Sets new color
		*
		* @param[in] r New value for @c red channel
		* @param[in] g New value for @c green channel
		* @param[in] b New value for @c blue channel
		* @param[in] a New value for @c alpha channel
		*
		* @note Value of 255 of alpha channel means fully opaque
		* @note While value of alpha channel 0 means fully transparent
		*
		* @see get
		*/
		void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		{
			data.r = r;
			data.g = g;
			data.b = b;
			data.a = a;
		}

		/**
		* \~English @brief Sets new color
		*
		* @param[in] value New value for color. Format RGBA with each channel taking 1 byte for the 4 bytes total
		*
		* @note Value of 255 of alpha channel means fully opaque
		* @note While value of alpha channel 0 means fully transparent
		*
		* @see get
		*/
		void set(uint32_t value)
		{
			data.col = value;
		}


		/**
		* \~English @brief Find the difference between two colors
		*
		* This function should be use to compare colors for similiarity. The less returned value is more similiar.
		*
		* @note Return 0 means the colors are identical, minus alpha
		*
		* @param[in] c1 First color to compare
		* @param[in] c2 Second color to compare
		*
		* @note Result of this function are range 0 - 195075
		*
		* @note This function uses separate square distance algorithm
		*
		* @see difference_including_alpha
		*/
		static uint32_t difference(color_t & c1, color_t & c2)
		{
			uint32_t ret = 0;
			int32_t part = 0;

			part = abs(static_cast<int32_t>(c1.data.r) - static_cast<int32_t>(c2.data.r)); ret += part * part;
			part = abs(static_cast<int32_t>(c1.data.g) - static_cast<int32_t>(c2.data.g)); ret += part * part;
			part = abs(static_cast<int32_t>(c1.data.b) - static_cast<int32_t>(c2.data.b)); ret += part * part;

			return ret;

		}


		/**
		* \~English @brief Find the difference between two colors including alpha for calculation
		*
		* This function should be use to compare colors for similiarity. The less returned value is more similiar.
		*
		* @note Return 0 means the colors are identical, including alpha component
		*
		* @param[in] c1 First color to compare
		* @param[in] c2 Second color to compare
		*
		* @note Result of this function are range 0 - 260100
		*
		* @note This function uses separate square distance algorithm
		*
		* @see difference
		*/
		static uint32_t difference_including_alpha(color_t & c1, color_t & c2)
		{
			uint32_t ret = 0;
			int32_t part = 0;

			part = abs(static_cast<int32_t>(c1.data.r) - static_cast<int32_t>(c2.data.r)); ret += part * part;
			part = abs(static_cast<int32_t>(c1.data.g) - static_cast<int32_t>(c2.data.g)); ret += part * part;
			part = abs(static_cast<int32_t>(c1.data.b) - static_cast<int32_t>(c2.data.b)); ret += part * part;
			part = abs(static_cast<int32_t>(c1.data.a) - static_cast<int32_t>(c2.data.a)); ret += part * part;

			return ret;

		}

	private:

		union
		{
			struct
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};

			uint32_t col;

		} data;
	};
}

#endif