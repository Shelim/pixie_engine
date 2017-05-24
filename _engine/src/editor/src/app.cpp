#include "common/injector.hpp"

#include "wx/wxprec.h"

#include "common/platform.hpp"
#include "common/manifest_app.hpp"

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

#include "editor/app.hpp"
#include "editor/frame/frame_main.hpp"

bool editor::app::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	wxSetWorkingDirectory(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()));

	wxSystemOptions::SetOption("msw.notebook.themed-background", 1);

	wxInitAllImageHandlers();

	auto injector = make_di(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).ToStdString());
	auto platform = injector.create<std::shared_ptr<engine::platform_t> >();

	platform->show_splashscreen(engine::virtual_path_t("splashscreen/editor.tga", engine::virtual_path_t::type_t::common));

	wxFrame* frame = new frame_main();
	SetTopWindow(frame);
	frame->Show();
	return true;

	return true;
}

DECLARE_APP(editor::app);
IMPLEMENT_APP(editor::app);