#ifndef LAUNCHER_APP_HPP
#define LAUNCHER_APP_HPP

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#include "wx/app.h"
#include "wx/log.h"
#include "wx/frame.h"
#include "wx/panel.h"
#include "wx/stattext.h"
#include "wx/menu.h"
#include "wx/layout.h"
#include "wx/msgdlg.h"
#include "wx/image.h"
#endif

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

namespace launcher
{
	class app : public wxApp
	{

	public:

		bool OnInit() final;
	};
}


#endif