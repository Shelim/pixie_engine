#include "editor/frame/frame_main.hpp"

editor::frame_main::frame_main(wxWindow* parent) : wxFrame(parent, wxID_ANY, _("wxAUI Test"), wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE)
{
	aui.SetManagedWindow(this);
	Maximize();

	// create several text controls
	wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	// add the panes to the manager
	aui.AddPane(text1, wxLEFT, wxT("Pane Number One"));
	aui.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
	aui.AddPane(text3, wxCENTER);

	// tell the manager to "commit" all the changes just made
	aui.Update();
}

editor::frame_main::~frame_main()
{
	// deinitialize the frame manager
	aui.UnInit();
}

wxBEGIN_EVENT_TABLE(editor::frame_main, wxFrame)

wxEND_EVENT_TABLE()
