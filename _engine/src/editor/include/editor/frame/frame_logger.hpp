#ifndef EDITOR_FRAME_LOGGER_HPP
#define EDITOR_FRAME_LOGGER_HPP
#pragma once

#include "common/logger.hpp"
#include <memory>

#include "wx/wxprec.h"

#include "editor/app.hpp"

#include "wx/wxprec.h"
#include "wx/app.h"
#include "wx/log.h"
#include "wx/frame.h"
#include "wx/panel.h"
#include "wx/stattext.h"
#include "wx/menu.h"
#include "wx/layout.h"
#include "wx/msgdlg.h"
#include "wx/image.h"
#include "wx/dcclient.h"
#include "wx/graphics.h"
#include "wx/image.h"
#include "wx/sound.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "wx/msgdlg.h"
#include "wx/button.h"

#include <fstream>
#include <string>
#include <map>
#include "pugixml.hpp"

namespace editor
{
	class frame_logger : public wxFrame
	{
	public:
		frame_logger(std::shared_ptr<engine::logger_t> logger, wxWindow* parent = nullptr);
		~frame_logger();

	private:

		wxDECLARE_EVENT_TABLE();
	};
}

#endif