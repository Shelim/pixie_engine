#ifndef EDITOR_FRAME_MAIN_HPP
#define EDITOR_FRAME_MAIN_HPP
#pragma once

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
#include "wx/textctrl.h"
#include "wx/aui/aui.h"
#include <fstream>
#include <string>
#include <map>
#include "pugixml.hpp"



namespace editor
{
	class frame_main : public wxFrame
	{
	public:
		frame_main(wxWindow* parent = nullptr);
		~frame_main();

	private:

		wxAuiManager aui;

		wxDECLARE_EVENT_TABLE();
	};
}

#endif