#ifndef ENGINE_COMMON_XML_HPP
#define ENGINE_COMMON_XML_HPP
#pragma once

#include "logger.hpp"
#include "utility/text/ustring.hpp"
#include <pugixml.hpp>

namespace engine
{
	/*
	inline bool load_xml(logger_t * logger, pugi::xml_document & doc, const ustring_t & data, const ustring_t & path) const
	{
		pugi::xml_parse_result res = doc.load_string(data.get_cstring());

		if (res.status == pugi::status_ok) return true;
		else
		{
			int prev = std::max(0, res.offset - 16);
			logger->p_err_raport("While parsing '#1#' got error: #2# [near '#3#']"_u, path, ustring_t::from_utf8(res.description()), data.substr(prev, 32));
			return false;
		}
	}

	template<class T> T get_xml_attribute(logger_t * logger, const pugi::xml_node & node, const char * attribute_name, T def_val = T(), const ustring_t & additional_log_data = _U(""), bool ignore_missing = false) const
	{
		pugi::xml_attribute attr = node.attribute(attribute_name);
		if (!attr)
		{
			if (!ignore_missing)
			{
				if (additional_log_data.is_empty())
					logger->p_err_raport("Missing '#1#' attribute in '#2#' node!"_u, ustring_t::from_utf8(attribute_name), ustring_t::from_utf8(node.name()));
				else
					logger->p_err_raport("Missing '#1#' attribute in '#2#' node (#3#)!"_u, ustring_t::from_utf8(attribute_name), ustring_t::from_utf8(node.name()), additional_log_data);
			}

			return def_val;
		}
		return from_string<T>(ustring_t::from_utf8(attr.value()));
	}

	template<class T> T get_xml_text(logger_t * logger, const pugi::xml_node & node, T def_val = T(), const ustring_t & additional_log_data = _U(""), bool ignore_missing = false) const
	{
		pugi::xml_text text = node.text()
		if (!text)
		{
			if (!ignore_missing)
			{
				if (additional_log_data.is_empty())
					logger->p_err_raport("Missing '#1#' value in '#2#' node!"_u, ustring_t::from_utf8(node.value()), ustring_t::from_utf8(node.name()));
				else
					logger->p_err_raport("Missing '#1#' value in '#2#' node (#3#)!"_u, ustring_t::from_utf8(node.value()), ustring_t::from_utf8(node.name()), additional_log_data);
			}

			return def_val;
		}
		return from_string<T>(ustring_t::from_utf8(text.as_string()));
	}
	*/
}

#endif