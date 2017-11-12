#include <wchar.h>
#include <locale>
#include <codecvt>
#include "utility/text/ustring.hpp"

//////////////////////////////////////////////////////////////////////////

engine::usymbol_t engine::to_upper(engine::usymbol_t item)
{
	return towupper(item);			// Is it cross platform?
}

engine::usymbol_t engine::to_lower(engine::usymbol_t item)
{
	return towlower(item);			// Is it cross platform?
}

bool engine::is_whitespace_ascii(char item)
{
	return item == ' ' || item == '\t' || item == '\n' || item == '\r';
}
bool engine::is_path_separator_ascii(char ch)
{
	return ch == '/' || ch == '\\';
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t engine::ustring_t::from_ascii(const char * str)
{
	engine::ustring_t ret;
	ret._str.reserve(strlen(str));
	while (*str)
	{
		if (*str < 0x80)
			ret._str += *str;
		else
			ret._str += '?';

		str++;
	}
	return ret;
}

engine::ustring_t engine::ustring_t::from_utf8(const char * str)
{
	engine::ustring_t ret;
	ret._str = str;
	return ret;
}

engine::ustring_t engine::ustring_t::from_wide(const wchar_t * str)
{
	engine::ustring_t ret;

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	ret._str = converter.to_bytes(str);

	return ret;
}

engine::ustring_t engine::ustring_t::from_symbol(usymbol_t str)
{
	engine::ustring_t ret;
	ret._encode(str);

	return ret;
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t engine::ustring_t::trim_string(const engine::ustring_t & str)
{
	uint_fast32_t left = 0;
	uint_fast32_t right = str._str.length();

	while (left < right && is_whitespace_ascii(str._str[left]))
		++left;
	while (right > 0 && is_whitespace_ascii(str._str[right]))
		--right;

	if (left < right)
		return ustring_t(str._str.substr(left, right - left));
	return ustring_t();
}

engine::ustring_t engine::ustring_t::left_trim_string(const engine::ustring_t & str)
{
	uint_fast32_t left = 0;
	uint_fast32_t right = str._str.length();

	while (left < right && is_whitespace_ascii(str._str[left]))
		++left;

	if (left < right)
		return ustring_t(str._str.substr(left, right - left));
	return ustring_t();
}

engine::ustring_t engine::ustring_t::right_trim_string(const engine::ustring_t & str)
{
	uint_fast32_t right = str._str.length();

	while (right > 0 && is_whitespace_ascii(str._str[right]))
		--right;

	if (0 < right)
		return ustring_t(str._str.substr(0, right));
	return ustring_t();
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t engine::ustring_t::to_upper_string(const engine::ustring_t & str)
{
	engine::ustring_t ret;
	ret._str.reserve(str._str.length());

	const char * it = str._str.c_str();

	while (*it)
	{
		ret._encode(engine::to_upper(_decode(it)));
	}

	return ret;
}

engine::ustring_t engine::ustring_t::to_lower_string(const engine::ustring_t & str)
{
	engine::ustring_t ret;
	ret._str.reserve(str._str.length());

	const char * it = str._str.c_str();

	while (*it)
	{
		ret._encode(engine::to_lower(_decode(it)));
	}

	return ret;
}
//////////////////////////////////////////////////////////////////////////

engine::ustring_t engine::ustring_t::replace(const ustring_t & from, const ustring_t & to) const
{
	if (from.is_empty())
		return *this;

	size_t start_pos = 0;

	engine::ustring_t ret = *this;

	while ((start_pos = ret._str.find(from._str, start_pos)) != -1)
	{
		ret._str.replace(start_pos, from._str.length(), to._str);
		start_pos += to._str.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t & engine::ustring_t::append(const engine::ustring_t & str)
{
	const char * it = str._str.c_str();

	while (*it)
	{
		_encode(_decode(it));
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t engine::ustring_t::substr_string(const engine::ustring_t & str, int_fast32_t pos)
{
	const uint_fast32_t length = str.len();

	if (pos >= 0)
	{
		if (pos > length)
		{
			pos = length;
		}
		return substr_string(str, pos, length - pos);
	}
	else
	{
		if (-pos > length)
		{
			pos = 0;
		}
		return substr_string(str, pos, -pos);
	}
}

engine::ustring_t engine::ustring_t::substr_string(const engine::ustring_t & str, int_fast32_t pos, int_fast32_t len)
{
	const int_fast32_t length = (int_fast32_t)str.len();
	ustring_t ret;

	uint_fast32_t first;
	uint_fast32_t last;

	if (pos > length) pos = length;
	if (-pos > length) pos = 0;

	if (pos < 0) first = length + pos;
	else first = pos;

	if (len < 0 && len > length) len = (int_fast32_t)length;

	if (len < 0) last = (length + len);
	else last = first + len;

	if (last <= first) return ret; // empty
	if (last > length) last = length;

	const char * it = str._str.c_str();

	for (uint_fast32_t i = 0; i < first; ++i)
	{
		it += _symbol_size(it);
	}

	for (uint_fast32_t i = first; i < last; ++i)
	{
		ret._encode(_decode(it));
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t engine::ustring_t::basename_string(const engine::ustring_t & str)
{
	uint_fast32_t len = str._str.length();

	if (len > 0 && is_path_separator_ascii(str._str[len - 1]))		// This cut last dir separator
		--len;

	uint_fast32_t i = len;
	while (i > 0)
	{
		if (is_path_separator_ascii(str._str[i - 1]))
			break;
		--i;
	}
	return engine::ustring_t(str._str.substr(i, len - i));
}

//////////////////////////////////////////////////////////////////////////

engine::ustring_t::ustring_t()
{
	// is_empty, this is intended
}

engine::ustring_t::ustring_t(const engine::ustring_t & other) : _str(other._str)
{

}

engine::ustring_t::ustring_t(engine::ustring_t && other) noexcept : _str(std::move(other._str))
{

}

engine::ustring_t & engine::ustring_t::operator=(const engine::ustring_t & other)
{
	_str = other._str;
	return (*this);
}

engine::ustring_t & engine::ustring_t::operator= (engine::ustring_t && other)
{
	_str = std::move(other._str);
	return (*this);
}

engine::ustring_t::~ustring_t()
{
	// is_empty, this is intended
}

//////////////////////////////////////////////////////////////////////////

uint_fast32_t engine::ustring_t::len() const
{
	const char * it = _str.c_str();

	uint_fast32_t ret = 0;
	while (*it)
	{
		++ret;
		it += _symbol_size(it);
	}

	return ret;
}

bool engine::ustring_t::is_empty() const
{
	return _str.empty();
}

//////////////////////////////////////////////////////////////////////////

int_fast32_t engine::ustring_t::index_of(usymbol_t symbol, int_fast32_t first /*= 0*/) const
{
	const char * it = _str.c_str();

	for (int i = 0; (*it) && i < first; i++)
		it += _symbol_size(it);

	uint_fast32_t index = first;

	while (*it)
	{
		if (_decode(it) == symbol)
		{
			return index;
		}
		index++;
	}
	return -1;
}

int_fast32_t engine::ustring_t::index_of(const engine::ustring_t & str, int_fast32_t first /*= 0*/) const
{
	const uint_fast32_t len = str.len();

	const char * it = _str.c_str();

	for (int i = 0; (*it) && i < first; i++)
		it += _symbol_size(it);

	const char * it2 = str._str.c_str();

	uint_fast32_t comp = 0;
	uint_fast32_t index = first;

	while (*it)
	{
		const engine::usymbol_t left = _decode(it);
		const engine::usymbol_t right = _decode(it2);

		++comp;

		if (left != right)
		{
			index += comp;
			comp = 0;
			it2 = str._str.c_str();
		}
		else
		{
			if (comp >= len)
				return index;
		}
	}
	return -1;
}

int_fast32_t engine::ustring_t::last_index_of(usymbol_t symbol, int_fast32_t last /*= 0*/) const
{
	const char * it = _str.c_str();

	uint_fast32_t index = 0;
	uint_fast32_t ret = -1;
	uint_fast32_t end = len() - last;

	while (*it)
	{
		if (!end--)
			break;

		if (_decode(it) == symbol)
		{
			ret = index;
		}
		index++;
	}
	return ret;
}

int_fast32_t engine::ustring_t::last_index_of(const engine::ustring_t & str, int_fast32_t last /*= 0*/) const
{
	const uint_fast32_t len = str.len();

	const char * it = _str.c_str();

	const char * it2 = str._str.c_str();

	uint_fast32_t comp = 0;
	uint_fast32_t index = 0;
	uint_fast32_t ret = -1;
	uint_fast32_t end = this->len() - last;

	while (*it)
	{
		const engine::usymbol_t left = _decode(it);
		const engine::usymbol_t right = _decode(it2);

		++comp;
		if (!end--)
			break;

		if (left != right)
		{
			index += comp;
			comp = 0;
			it2 = str._str.c_str();
		}
		else
		{
			if (comp >= len)
				ret = index;
		}
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

std::string engine::ustring_t::to_ascii() const
{
	std::string ret;
	ret.reserve(_str.length());

	const char * it = _str.c_str();

	while (*it)
	{
		engine::usymbol_t symbol = _decode(it);

		if (symbol < 0xC0) ret += (char)symbol;
		else ret += '?';
	}

	return ret;
}

std::string engine::ustring_t::to_utf8() const
{
	return _str;
}

std::wstring engine::ustring_t::to_wide() const
{
	std::wstring out;

	const char * in = _str.c_str();

	unsigned int codepoint;
	while (*in != 0)
	{
		unsigned char ch = static_cast<unsigned char>(*in);
		if (ch <= 0x7f)
			codepoint = ch;
		else if (ch <= 0xbf)
			codepoint = (codepoint << 6) | (ch & 0x3f);
		else if (ch <= 0xdf)
			codepoint = ch & 0x1f;
		else if (ch <= 0xef)
			codepoint = ch & 0x0f;
		else
			codepoint = ch & 0x07;
		++in;
		if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
		{
			if (codepoint > 0xffff)
			{
				out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
				out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
			}
			else if (codepoint < 0xd800 || codepoint >= 0xe000)
				out.append(1, static_cast<wchar_t>(codepoint));
		}
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////

const char * engine::ustring_t::get_cstring() const
{
	return _str.c_str();
}

//////////////////////////////////////////////////////////////////////////

void engine::ustring_t::_encode(engine::usymbol_t symbol)
{
	if (symbol < 0x80)
	{
		_str += (char)symbol;
	}
	else if (symbol < 0x800)
	{
		_str += (char)(0xC0 | (symbol >> 6));
		_str += (char)(0x80 | (symbol & 0x3F));
	}
	else if (symbol < 0x10000)
	{
		_str += (char)(0xE0 | (symbol >> 12));
		_str += (char)(0x80 | ((symbol >> 6) & 0x3F));
		_str += (char)(0x80 | (symbol & 0x3F));
	}
	else if (symbol < 0x200000)
	{
		_str += (char)(0xF0 | (symbol >> 18));
		_str += (char)(0x80 | ((symbol >> 12) & 0x3F));
		_str += (char)(0x80 | ((symbol >> 6) & 0x3F));
		_str += (char)(0x80 | (symbol & 0x3F));
	}
	else if (symbol < 0x4000000)
	{
		_str += (char)(0xF8 | (symbol >> 24));
		_str += (char)(0x80 | ((symbol >> 18) & 0x3F));
		_str += (char)(0x80 | ((symbol >> 12) & 0x3F));
		_str += (char)(0x80 | ((symbol >> 6) & 0x3F));
		_str += (char)(0x80 | (symbol & 0x3F));
	}
	else if (symbol < 0x8000000)
	{
		_str += (char)(0xFC | (symbol >> 30));
		_str += (char)(0x80 | ((symbol >> 24) & 0x3F));
		_str += (char)(0x80 | ((symbol >> 18) & 0x3F));
		_str += (char)(0x80 | ((symbol >> 12) & 0x3F));
		_str += (char)(0x80 | ((symbol >> 6) & 0x3F));
		_str += (char)(0x80 | (symbol & 0x3F));
	}
}

engine::usymbol_t engine::ustring_t::_decode(const char *& pos)
{
	const int size = _symbol_size(pos);
	register usymbol_t symbol = 0;

	if (size == 1)
	{
		symbol = *pos++;
	}
	else if (size == 2)
	{
		symbol += ((*pos++) & 0x1F) << 6;
		symbol += ((*pos++) & 0x3F);
	}
	else if (size == 3)
	{
		symbol += ((*pos++) & 0x0F) << 12;
		symbol += ((*pos++) & 0x3F) << 6;
		symbol += ((*pos++) & 0x3F);
	}
	else if (size == 4)
	{
		symbol += ((*pos++) & 0x07) << 18;
		symbol += ((*pos++) & 0x3F) << 12;
		symbol += ((*pos++) & 0x3F) << 6;
		symbol += ((*pos++) & 0x3F);
	}
	else if (size == 5)
	{
		symbol += ((*pos++) & 0x03) << 24;
		symbol += ((*pos++) & 0x3F) << 18;
		symbol += ((*pos++) & 0x3F) << 12;
		symbol += ((*pos++) & 0x3F) << 6;
		symbol += ((*pos++) & 0x3F);
	}
	else
	{
		symbol += ((*pos++) & 0x01) << 30;
		symbol += ((*pos++) & 0x3F) << 24;
		symbol += ((*pos++) & 0x3F) << 18;
		symbol += ((*pos++) & 0x3F) << 12;
		symbol += ((*pos++) & 0x3F) << 6;
		symbol += ((*pos++) & 0x3F);
	}

	return symbol;
}

uint_fast32_t engine::ustring_t::_symbol_size(const char * pos)
{
	register uint_fast32_t symbol = (unsigned char)*pos;

	if (symbol < 0xC0) return 1;
	if (symbol < 0xE0) return 2;
	if (symbol < 0xF0) return 3;
	if (symbol < 0xF8) return 4;
	if (symbol < 0xFC) return 5;
	return 6;
}

engine::ustring_t operator "" _u(const char * str, std::size_t)
{
	return engine::ustring_t::from_utf8(str);
}