#include "common/injector.hpp"

#include "wx/wxprec.h"

#include "common/manifest_app.hpp"
#include "common/logger_container.hpp"
#include "common/platform.hpp"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#include "wx/app.h"
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
#include "wx/filefn.h" 
#include <wx/sysopt.h>
#include <fstream>
#include <string>
#include <map>
#include "pugixml.hpp"

#include "launcher/App.hpp"

bool launcher::app::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	wxSetWorkingDirectory(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()));

	wxSystemOptions::SetOption("msw.notebook.themed-background", 1);

	wxInitAllImageHandlers();

	auto injector = make_di(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).ToStdString());

	auto platform = injector.create<std::shared_ptr<engine::platform_t> >();
	auto logger = injector.create<std::shared_ptr<engine::logger_t> >();
	auto logger_container = injector.create<std::shared_ptr<engine::logger_container_t> >();
	
	for (std::size_t i = 0; i < 100; i++)
	{
		logger_container->update();
		logger->p_msg(_U("Sample inside launcher!"));
	}
	int * a = nullptr;
	*a = 5;

//	new MainFrame;

	return true;
}

DECLARE_APP(launcher::app);
IMPLEMENT_APP(launcher::app);